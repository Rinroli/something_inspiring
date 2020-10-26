// Copyright (c) 2020 Rinroli

#include "func_file.h"
#include <queue>
// #include <string>
#include <vector>

using namespace std;

#define EPS 0.0001

class BinMatrix;

///// WAVE_CLUSTERS /////

WaveClusters::WaveClusters(const BinMatrix& matrix, ofstream& logs_field,
    Field* field)
    : logs_a(logs_field), matrix_inc(matrix)
{
    p_field = field;
    marks = matrix_inc.marks;

    findNextBegin();
    writeLog("INIT");
}

WaveClusters::~WaveClusters()
{
    writeLog("DELETE");
}

// One step of "burning" points.
bool WaveClusters::stepWave() {
    cur_step++;
    for (int i = 0; i < step_points.size(); ++i) {
        checkNeighbors(step_points[i]);
    }

    if (sec_step_points.size() == 0) {
        step_points.clear();
        return false;
    }
    step_points.swap(sec_step_points);
    sec_step_points.clear();
    return true;
}

// Check if neighbors can be marked.
void WaveClusters::checkNeighbors(int i) {
    for (int ind_poi = 0; ind_poi < matrix_inc.size; ++ind_poi) {
        if (matrix_inc[i][ind_poi]) {
            if (not marks[ind_poi]) {
                marks[ind_poi] = cur_step;
                p_field->setAPoint(ind_poi, cur_step);
                sec_step_points.push_back(ind_poi);
            }
        }
    }
}

// Find begining of the next cluster.
bool WaveClusters::findNextBegin() {
    writeLog("Begin findNextBegin");
    for (int i = 0; i < marks.size(); ++i) {
        if (marks[i] == 0) {
            marks[i] = 1;
            step_points.push_back(i);
            return true;
        }
    }
    return false;
}

// Create clusters by wave (dbscan) algorithm.
FindClusters WaveClusters::mainAlgorithm()
{
    writeLog("BEGIN CLUSTERING");
    string name_wave = "Wave clustering (delta = " + to_string(matrix_inc.delta) + ")";
    string name_dbscan = "DBSCAN (k=" + to_string(matrix_inc.k) + ", delta = " + to_string(matrix_inc.delta) + ")";
    FindClusters result_vector(logs_a, (matrix_inc.k == 1) ? name_wave : name_dbscan);
    int ind = 0;
    for (int dust_p = 0; dust_p < marks.size(); dust_p++) {
        if (marks[dust_p] == -1) {
            Cluster new_cluster(ind, logs_a, p_field);
            new_cluster += dust_p;
            ind++;
            result_vector += new_cluster;
        }
    }
    while (findNextBegin()) {
        cur_step = 1;
        while (stepWave()) {}
        Cluster new_cluster(ind, logs_a, p_field);
        for (int i = 0; i < marks.size(); ++i) {
            if (marks[i] > 0) {
                new_cluster += i;
                marks[i] = -1;
            }
        }
        result_vector += new_cluster;
        ind++;
    }
    writeLog("\tTurned out " + to_string(result_vector.numClusters()) + " clusters");
    cout << "Turned out " << to_string(result_vector.numClusters()) << " clusters. ";
    return result_vector;
}

// Write log-message with date-time note. 
void WaveClusters::writeLog(const string& message)
{
    logs_a << timeLog() << "WAVE: " << message << endl;
}

///// Tree /////

Tree::Tree(Point& pointt, ofstream& logs_al, double dist)
    : point(pointt), logs_a(logs_al), dist_parent(dist) {
    writeLog("INIT (of point #" + to_string(point.id) + ")");
}

Tree::~Tree() {
    neighbors.clear();
}

// Add new vertex to the current Tree.
void Tree::addVert(Point& new_point, double dist) {
    Tree new_vert(new_point, logs_a, dist);
    neighbors.push_back(new_vert);
}

// Getter for number of neighboring vertexes.
int Tree::numTrees() {
    return neighbors.size();
}

//Getter for neighboring vertex.
Tree Tree::operator[](int i) {
    return neighbors[i];
}

// Find vertex with index.
Tree* Tree::findIndex(int i) {
    for (int nei = 0; nei < neighbors.size(); nei++) {
        if (neighbors[nei].point.id == i) { return &neighbors[nei]; }

        Tree* result = neighbors[nei].findIndex(i);
        if (result != NULL) { return result; }
    }
    return NULL;
}

// Print tree to the file.
void Tree::displayTree(ofstream& out_f) {
    for (Tree nei : neighbors) {
        point.print(out_f);
        nei.point.print(out_f);
        out_f << endl << endl;
        nei.displayTree(out_f);
    }
}

// Print Tree as nested list.
void Tree::print(int indent) {
    string cur_indent(indent, '\t');
    cout << cur_indent << point << endl;
    for (Tree nei : neighbors) {
        nei.print(indent + 1);
    }
}

// Return vector with all edges.
vector<double> Tree::allDist() {
    vector<double> all_dist;
    all_dist.push_back(dist_parent);
    for (Tree nei : neighbors) {
        vector<double> res = nei.allDist();
        all_dist.insert(all_dist.end(), res.begin(), res.end());
    }
    return all_dist;
}

// Write log-message with date-time note.
void Tree::writeLog(const string& message) {
    logs_a << timeLog() << "TREE: " << message << endl;
}

///// KMEANS /////

KMeans::KMeans(int nn, Field& fieldd, ofstream& logs_al)
    : n(nn), field(fieldd), logs_a(logs_al) {
    writeLog("INIT");
    size_field = field.numPoints();
    for (int ind = 0; ind < n; ind++) {
        int point_ind = rand() % size_field;
        centers.push_back(field.points[point_ind].getCoord());
        clusters.push_back(vector<int>(size_field));
    }
}

// Make clustering using k-means algorithm.
FindClusters KMeans::mainAlgorithm() {
    writeLog("Begin mainAlgorithm");
    pointDistribution();
    while (findNewCenters()) {
        pointDistribution();
        step++;
    }
    FindClusters result(logs_a, "KMeans algorithm");
    int ind = 0;
    for (vector<int> cluster : clusters) {
        for (int a : cluster) {
            if (a) {
                vector<int> tmp_id_points;
                for (int id_poi = 0; id_poi < size_field; id_poi++) {
                    if (cluster[id_poi]) { tmp_id_points.push_back(id_poi); }
                }
                result += Cluster(ind, tmp_id_points, logs_a, &field);
                ind++;
                break;
            }
        }
    }
    writeLog("\tTurned out " + to_string(result.numClusters()) + " clusters in " + to_string(step) + " steps");
    cout << "Turned out " << to_string(result.numClusters()) << " clusters in " << step << " steps\n";
    return result;
}

// Find new centers as average of the cluster. 
// Return true, if there is any new ones, else false.
bool KMeans::findNewCenters() {
    writeLog("New step");
    bool result = false;
    for (int ind = 0; ind < centers.size(); ind++) {
        vector<double> tmp(2);
        int num_points = 0;
        for (int a_ind = 0; a_ind < size_field; a_ind++) {
            vector<double> point = field.points[a_ind].getCoord();
            int cur_mark = clusters[ind][a_ind];
            tmp[0] += cur_mark * point[0];
            tmp[1] += cur_mark * point[1];
            num_points += cur_mark;
        }
        tmp[0] /= num_points;
        tmp[1] /= num_points;
        if (distPoints(centers[ind], tmp) > EPS) { result = true; }
        centers[ind] = tmp;
    }
    return result;
}

// Find the nearest points to the centers.
void KMeans::pointDistribution() {
    for (int ind = 0; ind < clusters.size(); ind++) {
        for (int ind_p = 0; ind_p < size_field; ind_p++) {
            clusters[ind][ind_p] = 0;
        }
    }
    for (Point point : field.points) {
        clusters[nearestCenter(point)][point.id] = 1;
    }
}

// Find the nearest center to the given point.
int KMeans::nearestCenter(const Point& point) {
    double min_dist = INF;
    int nearest_center = -1;
    for (int ind_center = 0; ind_center < centers.size(); ind_center++) {
        double dist = distPoints(point, centers[ind_center]);
        if (dist < min_dist) {
            min_dist = dist;
            nearest_center = ind_center;
        }
    }
    return nearest_center;
}

// Write log-message with date-time note.
void KMeans::writeLog(const string& message) {
    logs_a << timeLog() << "KMEANS: " << message << endl;
}
