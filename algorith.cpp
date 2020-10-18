// Copyright (c) 2020 Rinroli

#include "func_file.h"
#include <queue>
// #include <string>
#include <vector>

using namespace std;

class BinMatrix;

///// WAVE_CLUSTERS /////

WaveClusters::WaveClusters(const BinMatrix &matrix, ofstream &logs_field,
                           Field *field)
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

bool WaveClusters::stepWave()
{
    /* One step of "burning" points. */
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

void WaveClusters::checkNeighbors(int i)
{
    /* Check if neighbors can be marked. */
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

bool WaveClusters::findNextBegin()
{
    /* Find begining of the next cluster. */
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
void WaveClusters::writeLog(const string &message)
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

// Print Tree as nested list.
void Tree::print(int indent) {
    string cur_indent(indent, '\t');
    cout << cur_indent << point << endl;
    for (Tree nei : neighbors) {
        nei.print(indent + 1);
    }
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
Tree *Tree::findIndex(int i) {
    for (int nei = 0; nei < neighbors.size(); nei++) {
        if (neighbors[nei].point.id == i) {
            return &neighbors[nei];
        }
        Tree *result = neighbors[nei].findIndex(i);
        if (result != NULL) {
            return result;
        }
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

vector<double> Tree::allDist() {
    vector<double> all_dist;
    all_dist.push_back(dist_parent);
    for (Tree nei : neighbors) {
        all_dist.push_back(nei.dist_parent);
        vector<double> res = nei.allDist();
        all_dist.insert(all_dist.end(), res.begin(), res.end());
    }
    return all_dist;
}

// Write log-message with date-time note.
void Tree::writeLog(const string& message) {
    logs_a << timeLog() << "TREE: " << message << endl;
}
