// Copyright (c) 2020 Rinroli

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
// #include <string>
#include <fstream>
#include <ctime>
#include <iomanip>

#include "func_file.h"

using namespace std;

#define N 75

///// POINT /////

Point::Point(double mX, double mY, double sX, double sY,
    int idd, ofstream& logs_field, int id_cloudd)
    : logs_f(logs_field)
{
    id = idd;
    x = normalPoint(mX, sX);
    y = normalPoint(mY, sY);
    id_cloud = id_cloudd;
}

void Point::changeTo(Point point)
{
    /* Changes point to the given one. */
    x = point.x;
    y = point.y;
    id = point.id;
    id_cloud = point.id_cloud;
    a = point.a;
    b = point.b;
    mark = point.mark;
}

vector<double> Point::getCoord()
{
    /* Return coords of the point. */
    vector<double> coords = { x, y };
    return coords;
}

void Point::print(ofstream& out_f)
{
    /* Save coords of the point to the file. */
    out_f << x << "  " << y << " ---- " << id_cloud << endl;
}

ostream& operator<<(ostream& out, const Point& point)
{
    /* Put point to the given stream. */
    out << "(" << point.x << ", " << point.y << ")";
    return out;
}

void Point::writeLog(string message)
{
    /* Write log-message with date-time note. */
    logs_f << timeLog() << "POINT(id:" << id << "): " << message << endl;
}

///// CLUSTER /////

Cluster::Cluster(int cur_id_cloud, ofstream& logs_field, Field* field)
    : logs_a{ logs_field }
{
    p_field_points = &(field->points);
    p_field = field;
    id = cur_id_cloud;
    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::~Cluster()
{
    id_points.clear();
    writeLog("DELETE");
}

int Cluster::numPoints()
{
    /* Return number of points in the cloud. */
    return id_points.size();
}

void Cluster::updateProp()
{
    /* Update Radius, Diameter and etc. of the Cluster(Cloud). */
    if (updated) { return; }

    double dist_f, tmp_dist;
    center.clear();
    for (int first : id_points) {
        dist_f = 0;
        for (int second : id_points) {
            tmp_dist = p_field->getDist(first, second);
            if (tmp_dist > dist_f) { dist_f = tmp_dist; }
        }
        if (dist_f > D) { D = dist_f; }
        if (dist_f == R) { center.push_back(first); }
        if (dist_f < R) {
            center.clear();
            center.push_back(first);
            R = dist_f;
        }
    }

    if (numPoints() > 0) {   // Box
        vector<double> some_point = (*p_field_points)[id_points[0]].getCoord();

        box[0] = some_point[0];
        box[1] = some_point[0];
        box[2] = some_point[1];
        box[3] = some_point[1];

        for (int i = 0; i < numPoints(); ++i) {
            some_point = (*p_field_points)[id_points[i]].getCoord();
            if (some_point[0] > box[1]) { box[1] = some_point[0]; }
            if (some_point[0] < box[0]) { box[0] = some_point[0]; }
            if (some_point[1] > box[3]) { box[3] = some_point[1]; }
            if (some_point[1] < box[2]) { box[2] = some_point[1]; }
        }
    }
}

double Cluster::getR()
{
    /* Getter for (R)adius. */
    updateProp();
    return R;
}

double Cluster::getD()
{
    /* Getter for (D)iameter. */
    updateProp();
    return D;
}

vector<double> Cluster::getBox()
{
    /* Getter for the box containing the cluster. */
    updateProp();
    return box;
}

vector<int> Cluster::getCenter()
{
    /* Getter for points from center of the cluster. */
    updateProp();
    return center;
}

Point Cluster::operator[](int i)
{
    /* Getter for point by its index. */
    return (*p_field_points)[id_points[i]];
}

Cluster& operator+=(Cluster& left, int i)
{
    /* Add point with absolut index i to the cluster; */
    left.id_points.push_back(i);
    return left;
}

void Cluster::coutInfo()
{
    /* Print info abput cluster. */
    updateProp();
    cout << "\t\t#" << id << "  Points " << numPoints() << ";  Box: x:[" << setw(8) << box[0] << ":"
        << setw(8) << box[1] << "] y:[" << setw(8) << box[2] << ":" << setw(8) << box[3] << "]" << endl
        << "\t\t\t\tD = " << setw(8) << getD()
        << "\tR = " << setw(8) << getR()
        << "\t\tCenter: [";
    for (int i = 0; i < center.size(); ++i) {
        cout << (*p_field_points)[center[i]] << ", ";
    }
    cout << "]" << endl;
}

void Cluster::writeLog(string message)
{
    /* Write log-message with date-time note. */
    logs_a << timeLog() << "CLUSTER(id:" << id << "): " << message << endl;
}

///// CLOUD /////

Cloud::Cloud(int cur_id_cloud, ofstream& logs_field, Field* field)
    : Cluster(cur_id_cloud, logs_field, field)
{
    p_field_points = &(field->points);
    p_field = field;
    id = cur_id_cloud;
    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cloud::~Cloud()
{
    id_points.clear();
    writeLog("DELETE");
}

void Cloud::writeLog(string message)
{
    /* Write log-message with date-time note. */
    logs_a << timeLog() << "CLOUD(id:" << id << "): " << message << endl;
}

///// BIN_MATRIXes /////

BinMatrix::BinMatrix(int sizee, double deltaa, int kk) {
    delta = deltaa;
    k = kk;
    size = sizee;
    matrix.resize(size);
    for (int i = 0; i < size; ++i) {
        matrix[i].resize(size);
    }
    marks = vector<int>(size);
}

BinMatrix::~BinMatrix() {
}

vector<bool>& BinMatrix::operator[](int i) {
    /* Getter for matrix line. */
    return matrix[i];
}

///// FIELD /////

Field::Field()
{
    cur_id_points = -1;
    cur_id_cloud = -1;
    readonly = false;

    logs.open("logs/logs_field.txt", ios_base::app);
    logs << endl
        << "New session" << timeLog() << endl;

    writeLog("INIT_FIELD");
}

Field::~Field()
{
    clouds.clear();
    fclusters.clear();
    points.clear();
    logs.close();
    if (logs_a) { logs_a.close(); }
    writeLog("DELETE");
}

bool Field::createCloud(double mX, double mY,
    double sX, double sY)
{
    /* Create new normally distributed cloud of points. */
    writeLog("CREATE CLOUD");
    Cloud new_cloud(nextCloud(), logs, this);
    if (!readonly) {
        for (int tmp = 0; tmp < N; tmp++) {
            Point point(mX, mY, sX, sY, nextPoint(), logs, new_cloud.id);
            points.push_back(point);
            new_cloud += point.id;
        }
        clouds.push_back(new_cloud);
        return true;
    }
    writeLog("Access denied");
    return false;
}

void Field::print(ofstream& out_f)
{
    /* Save clouds to the file. */
    for (int ind_cl = 0; ind_cl < clouds.size(); ind_cl++) {
        Cloud cur_cl = clouds[ind_cl];
        for (int ind_poi = 0; ind_poi < cur_cl.numPoints(); ind_poi++) {
            points[cur_cl[ind_poi].id].print(out_f);
        }
        out_f << endl
            << endl
            << endl;
    }
    writeLog("SAVE (clouds)");
}

void Field::print(int i, ofstream& out_f)
{
    /* Save clusters to the file. */
    FindClusters &cur_fd = getFCluster(i);
    for (int ind_cl = 0; ind_cl < cur_fd.numClusters(); ind_cl++) {
        Cluster cur_cl = cur_fd[ind_cl];
        for (int ind_poi = 0; ind_poi < cur_cl.numPoints(); ind_poi++) {
            points[cur_cl[ind_poi].id].print(out_f);
        }
        out_f << endl
            << endl
            << endl;
    }
    writeLog("SAVE (clusters, FD #" + to_string(cur_fd.id) + ")");
}

FindClusters& Field::getFCluster(int i)
{
    /* Getter for cluster by index. */
    return fclusters[i];
}

int Field::nextCloud()
{
    /* Return next unused cloud id. */
    cur_id_cloud++;
    return cur_id_cloud;
}

int Field::nextPoint()
{
    /* Return next unused point id. */
    cur_id_points++;
    return cur_id_points;
}

int Field::numClouds()
{
    /* Return number of clouds. */
    return cur_id_cloud + 1;
}

int Field::numPoints()
{
    /* Return number of points. */
    return cur_id_points + 1;
}

int Field::numFClusters()
{
    /* Return number of clusters. */
    return fclusters.size();
}

double Field::getDist(int ind1, int ind2)
{
    /* return distance between two points (by id) */
    return matrix[ind1][ind2];
}

Cloud& Field::operator[](int i)
{
    /* Provide access to clouds by index. */
    return clouds[i];
}

bool Field::ifReadonly()
{
    /* Say if modee is readonly. */
    return readonly;
}

bool Field::enterAnalysis()
{
    /* Enter readonly 'Analysis' mode */
    cout << "Entering 'Analysis' mode." << endl;
    try {
        readonly = true;
        updateD();
        logs_a.open("logs/logs_algorithm.txt", ios_base::app);
        logs_a << endl
            << "New session" << timeLog() << endl;
    }
    catch (...) {
        writeLog("\tSomething went wrong.");
        return false;
    }
    writeLog("CHANGED MODE - ANALYSIS NOW");
    return true;
} 

void Field::setAPoint(int i, int value)
{
    /* Set value of the mark. */
    points[i].a = value;
}

void Field::updateD()
{
    /* Update distance matrix if field was changed. */
    double dist = 0;
    matrix.resize(cur_id_points + 1);
    for (int i = 0; i < cur_id_points + 1; ++i) {
        matrix[i].resize(cur_id_points + 1);
    }

    for (int first = 0; first < cur_id_points + 1; ++first) {
        for (int second = first; second < cur_id_points + 1; ++second) {
            dist = distPoints(points[first], points[second]);
            matrix[first][second] = dist;
            matrix[second][first] = dist;
        }
    }
    writeLog("MATRIX UPDATED");
}

void Field::addFC(FindClusters new_fc)
{
    /* Add new FindClusters to the vector with them. */
    new_fc.setID(fclusters.size());
    fclusters.push_back(new_fc);
    writeLog("ADD FIND_CLUSTERS - Source " + new_fc.source);
}

BinMatrix& Field::getBinMatrix(int i) {
    /* Get binary matrix by index.

    -1 is the last one.
    */
    if (i == -1) { return bin_matrixes[bin_matrixes.size() - 1]; }
    return bin_matrixes[i];
}

int Field::numBinMatrix() {
    /* Return number of binary matrixes. */
    return bin_matrixes.size();
}

void Field::binMatrix(double delta)
{
    /* Create matrix (graph) with info about incident points. */
    writeLog("Begin creating binary matrix (" + to_string(delta) + ")");
    BinMatrix matrix_inc(numPoints(), delta);
    for (int ind_poi = 0; ind_poi < numPoints(); ++ind_poi) {
        matrix_inc[ind_poi][ind_poi] = 0;
        for (int ind_poi_sec = ind_poi; ind_poi_sec < numPoints(); ++ind_poi_sec) {
            if (ind_poi != ind_poi_sec & matrix[ind_poi][ind_poi_sec] < delta) {
                matrix_inc[ind_poi][ind_poi_sec] = 1;
                matrix_inc[ind_poi_sec][ind_poi] = 1;
            }
            else {
                matrix_inc[ind_poi][ind_poi_sec] = 0;
                matrix_inc[ind_poi_sec][ind_poi] = 0;
            }
        }
    }
    bin_matrixes.push_back(matrix_inc);
    writeLog("\tCREATE binary matrix");
}

void Field::binDBMatrix(double delta, int k) {
    /* Create matrix (graph) with info about incident points and neighbors. */
    writeLog("Begin creating binary db matrix (" + to_string(delta) + ", " + to_string(k) + ")");
    BinMatrix matrix_inc(numPoints(), delta, k);
    int tmp;
    for (int ind_poi = 0; ind_poi < numPoints(); ++ind_poi) {
        tmp = 0;
        matrix_inc[ind_poi][ind_poi] = 0;
        for (int ind_poi_sec = ind_poi; ind_poi_sec < numPoints(); ++ind_poi_sec) {
            if (ind_poi != ind_poi_sec & matrix[ind_poi][ind_poi_sec] < delta) {
                matrix_inc[ind_poi][ind_poi_sec] = 1;
                matrix_inc[ind_poi_sec][ind_poi] = 1;
                tmp++;
            }
            else {
                matrix_inc[ind_poi][ind_poi_sec] = 0;
                matrix_inc[ind_poi_sec][ind_poi] = 0;
            }
        }
        if (tmp < k) {
            matrix_inc.marks[ind_poi] = -1;
            points[ind_poi].mark = "dust";
        }
    }

    for (int ind_poi = 0; ind_poi < numPoints(); ++ind_poi) {
        if (matrix_inc.marks[ind_poi] == -1) {
            for (int ind_poi_sec = 0; ind_poi_sec < numPoints(); ++ind_poi_sec) {
                if (matrix_inc.marks[ind_poi_sec] == 0 & matrix_inc[ind_poi][ind_poi_sec]) {
                    matrix_inc.marks[ind_poi] = -2;
                    break;
                }
            }
        }
    }
    for (int j = 0; j < numPoints(); ++j) {
        if (matrix_inc.marks[j] == -2) {
            points[j].mark  = "peripheral";
            matrix_inc.marks[j] = 0;
        }
    }
    bin_matrixes.push_back(matrix_inc);
    writeLog("\tCREATE binary matrix with neighbors");
}

void Field::drawBinGraph(int i) {
    /* Print edges to the file graph_edges.plt. */
    ofstream graph_edges("data/graph_edges.plt");
    ofstream graph_points("data/graph_points.plt");

    BinMatrix& matrix_inc = getBinMatrix(i);
    for (int poi = 0; poi < numPoints(); ++poi) {
        if (points[poi].mark == "dust") {
            points[poi].print(graph_points);
            continue;
        }
        for (int nei_p = 0; nei_p < numPoints(); nei_p++) {
            if ((points[poi].mark == "peripheral" | poi < nei_p) &
                matrix_inc[poi][nei_p] & points[nei_p].mark == "base") {
                points[poi].print(graph_edges);
                points[nei_p].print(graph_edges);
                graph_edges << endl << endl;
            }
        }
    }
    graph_edges.close();

    graph_points << endl << endl;
    for (Point point : points) {
        if (point.mark == "peripheral") {
            point.print(graph_points);
        }
    }
    graph_points << endl << endl;
    for (Point point : points) {
        if (point.mark == "base") {
            point.print(graph_points);
        }
    }
    graph_points.close();
}

void Field::writeLog(string message)
{
    /* Write log-message with date-time note. */
    logs << timeLog() << "FIELD: " << message << endl;
}

///// FIND_CLUSTERS /////

FindClusters::FindClusters(ofstream& logs_al, const string sourcee)
    : logs_a(logs_al), source(sourcee)
{
    writeLog("INIT");
}

FindClusters::~FindClusters()
{
    clusters.clear();
    writeLog("DELETE");
}

Cluster& FindClusters::operator[](int i)
{
    /* Getter for the cluster from vector. */
    return clusters[i];
}

void FindClusters::setID(int i) {
    /* Setter for id. */
    id = i;
}

void FindClusters::coutInfo()
{
    /* Cout info about FindClusters to the standart output. */
    cout << "\t#" << id << " Source: " << source << ", "
        << clusters.size() << " clusters." << endl;
    for (Cluster cl : clusters) {
        cl.coutInfo();
    }
}

void operator+=(FindClusters& left, Cluster new_cluster)
{
    /* Simple addition of new cluster to FindClusters. */
    left.clusters.push_back(new_cluster);
}

int FindClusters::numClusters()
{
    /* Return number of clusters. */
    return clusters.size();
}

void FindClusters::writeLog(string message)
{
    /* Write log-message with date-time note. */
    logs_a << timeLog() << "FIND_CLUSTERS: " << message << endl;
}

///// FUNCTIONS /////

double normalPoint(double mu, double sigma)
{
    /*   Normal distribution. */
    double sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += getRandom();
    }
    return mu + (sum / N) * sigma;
}

double getRandom()
{
    /*   Return random double in [-1, 1]. */
    int po = rand() % 10000 - 5000;
    double u_1 = (static_cast<double>(po) / 10000);
    return u_1;
}

string timeLog()
{
    /* Return clear string with time info for logs. */
    time_t seconds = time(0);
    tm* timeinfo = localtime(&seconds);
    string tmp = to_string(timeinfo->tm_mday);
    tmp += "." + to_string(timeinfo->tm_mon + 1) + " -- " + to_string(timeinfo->tm_hour);
    tmp += ":" + to_string(timeinfo->tm_min) + " -> ";
    return tmp;
}

double distPoints(Point f_poi, Point s_poi)
{
    /*  Return distance between two points.  */
    return sqrt((f_poi.x - s_poi.x) * (f_poi.x - s_poi.x) +
        ((f_poi.y - s_poi.y) * (f_poi.y - s_poi.y)));
}