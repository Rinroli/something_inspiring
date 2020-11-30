// Copyright (c) 2020 Rinroli

#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <cmath>
#include <iostream>
// #include <string>
#include <fstream>
#include <iomanip>

#include "func_file.h"

using namespace std;

#define INF 100000007
#define PI 3.14159265

class Tree;

///// POINT /////

Point::Point(double mX, double mY, double sX, double sY,
    int idd, ofstream& logs_field, int id_cloudd, int nu_points)
    : logs_f(logs_field)
{
    id = idd;
    x = normalPoint(mX, sX, nu_points);
    y = normalPoint(mY, sY, nu_points);
    id_cloud = id_cloudd;
}

Point::Point(const vector<double>& coords, int idd, int id_cloudd, ofstream& logs)
: x(coords[0]), y(coords[1]), id(idd), id_cloud(id_cloudd), logs_f(logs)
{
}

// Changes point to the given one.
void Point::changeTo(Point point) {
    x = point.x;
    y = point.y;
    id = point.id;
    id_cloud = point.id_cloud;
    a = point.a;
    b = point.b;
    mark = point.mark;
}

// Set new id.
void Point::setID(int idd) {
    id = idd;
}

// Set new coords.
void Point::setCoords(double new_x, double new_y) {
    x = new_x; y = new_y;
}

// Return coords of the point.
vector<double> Point::getCoord() {
    vector<double> coords = { x, y };
    return coords;
}

// Return X-coord of the point.
double Point::getX() {
    return x;
}

// Return Y-coord of the point.
double Point::getY() {
    return y;
}

// Rotate point relative to the center on alpha angle in grad.
void Point::rotatePoint(double alpha) {
    alpha = alpha * PI / 180;
    double new_x = x * cos(alpha) - y * sin(alpha);
    double new_y = x * sin(alpha) + y * cos(alpha);
    x = new_x; y = new_y;
}

// Move point on vector (new_x, new_y).
void Point::movePoint(double new_x, double new_y) {
    x += new_x;
    y += new_y;
}

// Zoom in or zoom out k times (relative to the center) point
void Point::zoomPoint(double k) {
    x *= k;
    y *= k;
}

// Save coords of the point to the file.
void Point::print(ofstream& out_f) {
    out_f << x << "  " << y << " ---- " << id_cloud << endl;
}

// Put point to the given stream.
ostream& operator<<(ostream& out, const Point& point) { 
    out << "(" << point.x << ", " << point.y << ")";
    return out;
}

// Write log-message with date-time note.
void Point::writeLog(const string &message) {
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

Cluster::Cluster(int idd, const vector<int> &points, ofstream& logs_field, Field* field)
    : logs_a{ logs_field }, id_points(points)
{
    p_field_points = &(field->points);
    p_field = field;
    id = idd;

    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::~Cluster()
{
    id_points.clear();
    writeLog("DELETE");
}

// Return number of points in the cloud.
int Cluster::numPoints() {
    return id_points.size();
}

// Update Radius, Diameter and etc. of the Cluster(Cloud). 
void Cluster::updateProp() {
    if (updated) { return; }

    double tmp_dist;
    center.clear();
    for (int first : id_points) {
        double dist_f = 0;
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

// Find average of coords.
vector<double> Cluster::findAverage() {
    vector<double> aver(2);
    for (int id_point : id_points) {
        vector<double> coords = (*p_field_points)[id_point].getCoord();
        aver[0] += coords[0];
        aver[1] += coords[1];
    }
    aver[0] /= numPoints();
    aver[1] /= numPoints();
    return aver;
}

// Getter for (R)adius.
double Cluster::getR() {
    updateProp();
    return R;
}

// Getter for (D)iameter.
double Cluster::getD() {
    updateProp();
    return D;
}

// Getter for the box containing the cluster.
vector<double> Cluster::getBox() {
    updateProp();
    return box;
}

// Getter for points from center of the cluster.
vector<int> Cluster::getCenter() {
    updateProp();
    return center;
}

// Getter for point by its index.
Point& Cluster::operator[](int i) {
    return (*p_field_points)[id_points[i]];
}

// Add point with absolut index i to the cluster.
Cluster& operator+=(Cluster& left, int i)
{
    left.id_points.push_back(i);
    return left;
}

Cluster& Cluster::operator=(const Cluster& new_cluster) {
    if (this == &new_cluster) {
        return *this;
    }
    id = new_cluster.id;
    id_points = new_cluster.id_points;
    updated = new_cluster.updated;
    R = new_cluster.R; D = new_cluster.D;
    center = new_cluster.center;
    box = new_cluster.box;
    p_field = new_cluster.p_field;
    p_field_points = new_cluster.p_field_points;
    return *this;
}

// Add point to the cluster.
Cluster& operator+=(Cluster& left, Point point)
{
    left.id_points.push_back(point.id);
    return left;
}

// Clear vector of points' id.
void Cluster::clear() {
    id_points.clear();
}

// Print info abput cluster.
void Cluster::coutInfo() {
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

void Cluster::printGnu(ofstream& ofile) {
    for (int point_id : id_points) {
        (*p_field).getPoint(point_id).print(ofile);
    }
}

// Write log-message with date-time note.
void Cluster::writeLog(const string &message) {
    logs_a << timeLog() << "CLUSTER(id:" << id << "): " << message << endl;
}

///// BUFER /////

Buffer::Buffer(Field* p_fieldd)
: logs_f(p_fieldd->logs), p_field(p_fieldd)
{
    writeLog("INIT");
}

Buffer::~Buffer()
{
    points.clear();
    writeLog("DELETE");
}

// Addition of id points from cluster to buffer.
void Buffer::addCluster(Cluster& new_cluster) {
    for (int id_in_cl = 0; id_in_cl < new_cluster.id_points.size(); id_in_cl++) {
        Point new_point(new_cluster[id_in_cl]);
        new_point.setID(id_in_cl);
        points.push_back(new_point);
    }
    writeLog("ADD_POINTS_FROM_CLOUD #" + to_string(new_cluster.id));
}

// Simple addition of new cluster to buffer.
void operator+=(Buffer& left, Cluster& new_cluster) {
    left.addCluster(new_cluster);
}

// Rotate points relative to the center by an alpha angle
void Buffer::rotatePoints(double alpha) {
    for (int ind_point = 0; ind_point < points.size(); ind_point++) {
        points[ind_point].rotatePoint(alpha);
    }
    writeLog("ROTATE (" + to_string(alpha) + ")");
}

// Move points of the buffer.
void Buffer::movePoints(double x, double y) {
    for (int ind_point = 0; ind_point < points.size(); ind_point++) {
        points[ind_point].movePoint(x, y);
    }
    writeLog("MOVE (" + to_string(x) + ", " + to_string(y) + ")");
}

// Zoom in or zoom out k times (relative to the center) points from the buffer.
void Buffer::zoomPoints(double k) {
    for (int ind_point = 0; ind_point < points.size(); ind_point++) {
        points[ind_point].zoomPoint(k);
    }
    writeLog("ZOOM (" + to_string(k) + ")");
}

// Delete all points from the buffer.
void Buffer::deletePoints() {
    points.clear();
}

// Check if buffer is empty.
bool Buffer::isEmpty() {
    if (points.size() == 0) { return true; }
    return false;
}

// Print some info about buffer to standart output.
void Buffer::coutInfo() {
    cout << "Buffer:" << endl;
    for (Point point : points) {
        cout << "\t" << point << endl;
    }
}

// Copy all points from buffer to field as one cloud.
void Buffer::putToField() {
    p_field->createCloud(points);
    writeLog("PUT_TO_FIELD");
}

// Write log-message with date-time note.
void Buffer::writeLog(const string& message) {
    logs_f << timeLog() << "BUFFER: " << message << endl;
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

Cloud::Cloud(int idd, const vector<int>& points, ofstream& logs_field, Field* field)
    : Cluster(idd, points, logs_field, field)
{
    p_field_points = &(field->points);
    p_field = field;
    id = idd;

    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cloud::~Cloud()
{
    id_points.clear();
    writeLog("DELETE");
}

// Write log-message with date-time note.
void Cloud::writeLog(const string &message) {
    logs_a << timeLog() << "CLOUD(id:" << id << "): " << message << endl;
}

///// BIN_MATRIX /////

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

// Getter for matrix line.
vector<bool>& BinMatrix::operator[](int i) {
    return matrix[i];
}

///// FIELD /////

Field::Field(vector<bool> if_logs, vector<string> name_logs)
: buffer{this} {
    readonly = false;

    if (if_logs[0]) {
        logs.open("logs/" + name_logs[0], ios_base::app);
        logs << endl << "New session" << endl;
        writeLog("INIT");
    }
    if (if_logs[3]) {
        logs_a.open("logs/" + name_logs[3], ios_base::app);
        logs_a << endl << "New session" << timeLog() << endl;
    }
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

// Create new normally distributed cloud of points.
bool Field::createCloud(double mX, double mY,
    double sX, double sY, int nu_points) {
    writeLog("CREATE CLOUD (normally distributed)");
    if (!readonly) {
        Cloud new_cloud(numClouds(), logs, this);
        for (int tmp = 0; tmp < nu_points; tmp++) {
            Point point(mX, mY, sX, sY, numPoints(), logs, new_cloud.id, nu_points);
            points.push_back(point);
            new_cloud += point.id;
        }
        clouds.push_back(new_cloud);
        return true;
    }
    writeLog("Access denied");
    return false;
}

// Create new normally distributed cloud of points.
bool Field::createCloud(vector<Point> new_points) {
    writeLog("CREATE CLOUD (from buffer)");
    if (!readonly) {
        Cloud new_cloud(numClouds(), logs, this);
        for (Point point : new_points) {
            point.setID(numPoints());
            points.push_back(point);
            new_cloud += point.id;
        }
        clouds.push_back(new_cloud);
        return true;
    }
    writeLog("Access denied");
    return false;
}

// Save clouds to the file.
void Field::print(ofstream& out_f)
{
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

// Show info about buffer.
void Field::showBuffer() {
    if (buffer.isEmpty()) {
        cout << "Empty buffer!" << endl;
    }
    buffer.coutInfo();
    writeLog("SHOWBUFFER");
}

// Add cloud to the buffer.
bool Field::addToBuffer(int ind_cl) {
    if (numClouds() == 0) {
        cout << "No clouds." << endl;
        return false;
    }
    if (ind_cl == -1) { buffer += clouds[numClouds() - 1]; }
    else { buffer += clouds[ind_cl]; }
    return true;
}

// Copy buffer to the field.
bool Field::putBuffer() {
    if (buffer.isEmpty()) {
        cout << "Empty buffer!" << endl;
        return false;
    }
    buffer.putToField();
    return true;
}

// Rotate buffer.
bool Field::rotateBuffer(double alpha) {
    if (buffer.isEmpty()) {
        cout << "Empty buffer!" << endl;
        return false;
    }
    buffer.rotatePoints(alpha);
    return true;
}

// Move buffer.
bool Field::moveBuffer(double x, double y) {
    if (buffer.isEmpty()) {
        cout << "Empty buffer!" << endl;
        return false;
    }
    buffer.movePoints(x, y);
    return true;
}

// Zoom buffer.
bool Field::zoomBuffer(double k) {
    if (buffer.isEmpty()) {
        cout << "Empty buffer!" << endl;
        return false;
    }
    buffer.zoomPoints(k);
    return true;
}

// Empty buffer.
bool Field::emptyBuffer() {
    if (buffer.isEmpty()) {
        cout << "Buffer is already empty!" << endl;
        return false;
    }
    buffer.deletePoints();
    return true;
}

// Save clusters to the file.
void Field::print(int i, ofstream& out_f)
{
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

// Getter for cluster by index.
FindClusters& Field::getFCluster(int i) {
    return fclusters[i];
}

// Return number of clouds.
int Field::numClouds() {
    return clouds.size();
}

// Return number of points.
int Field::numPoints() {
    return points.size();
}

// Return number of clusters.
int Field::numFClusters() {
    return fclusters.size();
}

// Return distance between two points (both by id)
double Field::getDist(int ind1, int ind2) {
    return matrix[ind1][ind2];
}

// Return distance between two points (by point and id)
double Field::getDist(Point point1, int ind2) {
    return matrix[point1.id][ind2];
}

// Provide access to clouds by index.
Point& Field::operator[](int i) {
    if (i == -1) { return points[numPoints() - 1]; }
    return points[i];
}

// Say if modee is readonly.
bool Field::ifReadonly() {
    return readonly;
}

// Enter readonly 'Analysis' mode.
bool Field::enterAnalysis()
{
    cout << "Entering 'Analysis' mode." << endl;
    if (numPoints() == 0) {
        cout << "There is no points. You can do nothing with them." << endl;
        throw -2;
    }
    try {
        readonly = true;
        updateD();
    }
    catch (...) {
        writeLog("\tSomething went wrong.");
        return false;
    }
    writeLog("CHANGED MODE - ANALYSIS NOW");
    return true;
} 

// Set value of the mark.
void Field::setAPoint(int i, int value) {
    points[i].a = value;
}

// Update distance matrix if field was changed.
void Field::updateD() {
    double dist;
    matrix.resize(numPoints());
    for (int i = 0; i < numPoints(); ++i) {
        matrix[i].resize(numPoints());
    }

    for (int first = 0; first < numPoints(); ++first) {
        for (int second = first; second < numPoints(); ++second) {
            dist = distPoints(points[first], points[second]);
            matrix[first][second] = dist;
            matrix[second][first] = dist;
        }
    }
    writeLog("MATRIX UPDATED");
}

// Add new FindClusters to the vector with them.
void Field::addFC(FindClusters new_fc) {
    new_fc.setID(fclusters.size());
    fclusters.push_back(new_fc);
    writeLog("ADD FIND_CLUSTERS - Source " + new_fc.source);
}

// Get binary matrix by index.
// >> -1 is the last one.
BinMatrix& Field::getBinMatrix(int i) {
    if (i == -1) { return bin_matrixes[bin_matrixes.size() - 1]; }
    return bin_matrixes[i];
}

// Getter for points by index.
Point& Field::getPoint(int i) {
    return points[i];
}

// Getter for clouds by index.
Cloud& Field::getCloud(int i) {
    return clouds[i];
}

// Return number of binary matrixes.
int Field::numBinMatrix() {
    return bin_matrixes.size();
}

// Create matrix (graph) with info about incident points.
void Field::binMatrix(double delta) {
    writeLog("Begin creating binary matrix (" + to_string(delta) + ")");
    BinMatrix matrix_inc(numPoints(), delta);
    for (int ind_poi = 0; ind_poi < numPoints(); ++ind_poi) {
        matrix_inc[ind_poi][ind_poi] = 0;
        for (int ind_poi_sec = ind_poi; ind_poi_sec < numPoints(); ++ind_poi_sec) {
            if ((ind_poi != ind_poi_sec) & (matrix[ind_poi][ind_poi_sec] < delta)) {
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

// Create matrix (graph) with info about incident points and neighbors.
void Field::binDBMatrix(double delta, int k) {
    writeLog("Begin creating binary db matrix (" + to_string(delta) + ", " + to_string(k) + ")");
    BinMatrix matrix_inc(numPoints(), delta, k);
    for (int ind_poi = 0; ind_poi < numPoints(); ++ind_poi) {
        int tmp = 0;
        matrix_inc[ind_poi][ind_poi] = 0;
        for (int ind_poi_sec = ind_poi; ind_poi_sec < numPoints(); ++ind_poi_sec) {
            if ((ind_poi != ind_poi_sec) & (matrix[ind_poi][ind_poi_sec] < delta)) {
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
                if ((matrix_inc.marks[ind_poi_sec] == 0) & (matrix_inc[ind_poi][ind_poi_sec])) {
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

// Print edges to the file graph_edges.plt.
void Field::drawBinGraph(int i) {
    ofstream graph_edges("data/graph_edges.plt");
    ofstream graph_points("data/graph_points.plt");

    BinMatrix& matrix_inc = getBinMatrix(i);
    for (int poi = 0; poi < numPoints(); ++poi) {
        if (points[poi].mark == "dust") {
            points[poi].print(graph_points);
            continue;
        }
        for (int nei_p = 0; nei_p < numPoints(); nei_p++) {
            if (((points[poi].mark == "peripheral") | (poi < nei_p)) &
                    (matrix_inc[poi][nei_p]) & (points[nei_p].mark == "base")) {
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

// Create minimal spanning tree.
void Field::minSpanTree() {
    writeLog("Begin minSpanTree");
    p_tree = new Tree(points[0], logs_a);

    vector<bool> already_taken(numPoints());
    already_taken[0] = true;
    int min_ind, min_taken_ind;

    for (int step = 0; step < numPoints() - 1; step++) {
        double min_dist = INF;
        for (int ind = 0; ind < numPoints(); ind++) {
            if (!already_taken[ind]) {
                for (int taken_poi = 0; taken_poi < numPoints(); taken_poi++) {
                    if (already_taken[taken_poi]) {
                        if (getDist(ind, taken_poi) < min_dist) {
                            min_dist = getDist(ind, taken_poi);
                            min_ind = ind; min_taken_ind = taken_poi;
                        }
                    }
                }
            }
        }
        if (min_taken_ind == 0) { p_tree->addVert(points[min_ind], min_dist); }
        else { (*(p_tree->findIndex(min_taken_ind))).addVert(points[min_ind], min_dist); }
        already_taken[min_ind] = true;
    }

    ofstream graph("data/tree_data.plt");
    p_tree->displayTree(graph);
}

// Write log-message with date-time note.
void Field::writeLog(const string &message)
{
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

// Getter for the cluster from vector.
Cluster& FindClusters::operator[](int i) {
    return clusters[i];
}

// Setter for id.
void FindClusters::setID(int i) {
    id = i;
}

// Cout info about FindClusters to the standart output.
void FindClusters::coutInfo() {
    cout << "\t#" << id << " Source: " << source << ", "
        << clusters.size() << " clusters." << endl;
    for (Cluster cl : clusters) {
        cl.coutInfo();
    }
}

// Simple addition of new cluster to FindClusters
void operator+=(FindClusters& left, Cluster new_cluster) {
    left.clusters.push_back(new_cluster);
}

// Return number of clusters.
int FindClusters::numClusters() {
    return clusters.size();
}

// Write log-message with date-time note.
void FindClusters::writeLog(const string &message) {
    logs_a << timeLog() << "FIND_CLUSTERS: " << message << endl;
}

///// FUNCTIONS /////

// Normal distribution.
double normalPoint(double mu, double sigma, int nu_points) {
    double sum = 0;
    for (int i = 0; i < nu_points; ++i) {
        sum += getRandom();
    }
    return mu + (sum / nu_points) * sigma;
}

// Return random double in [-1, 1].
double getRandom() {
    int po = rand() % 10000 - 5000;
    double u_1 = (static_cast<double>(po) / 10000);
    return u_1;
}


// Return random integer from [0, max_val)
int randInt(int max_val) {
    return rand() % max_val;
}

// Return clear string with time info for logs.
string timeLog() {
    time_t seconds = time(0);
    tm* timeinfo = localtime(&seconds);
    string tmp = to_string(timeinfo->tm_mday);
    tmp += "." + to_string(timeinfo->tm_mon + 1) + " -- " + to_string(timeinfo->tm_hour);
    tmp += ":" + to_string(timeinfo->tm_min) + " -> ";
    return tmp;
}

// Return distance between two points.
double distPoints(Point f_poi, Point s_poi) {
    return sqrt((f_poi.x - s_poi.x) * (f_poi.x - s_poi.x) +
        ((f_poi.y - s_poi.y) * (f_poi.y - s_poi.y)));
}

// Return distance between point and vector with coords.
double distPoints(Point point, const vector<double> &center) {
    return sqrt((point.x -center[0]) * (point.x -center[0]) +
        ((point.y - center[1]) * (point.y - center[1])));
}

// Return distance between two vectors with coords.
double distPoints(const vector<double>& center1, const vector<double>& center2) {
    return sqrt((center1[0] - center2[0]) * (center1[0] - center2[0]) +
        ((center1[1] - center2[1]) * (center1[1] - center2[1])));
}

// Return distance between vectors.
double distVectors(const vector<double>& vector_1, const vector<double>& vector_2) {
    if (vector_1.size() != vector_2.size()) {cout << "!!!" << endl;return 0;}
    double tmp_sum = 0;
    for (int i = 0; i < vector_1.size(); i++) {
        tmp_sum += (vector_1[i] - vector_2[i]) * (vector_1[i] - vector_2[i]);
    }
    return sqrt(tmp_sum);
}