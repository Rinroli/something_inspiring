// Copyright (c) 2020 Rinroli

#include "Tree.h"

Tree::Tree(Point& pointt, ofstream& logs_al, double dist)
    : point(pointt), logs_a(logs_al), dist_parent(dist) {
    writeLog("INIT (of point #" + to_string(point.id) + ")");
}

Tree::Tree(Point& pointt, Tree* f_tree, Tree* s_tree, ofstream& logs_al)
    : point(pointt), logs_a(logs_al) {
    neighbors.push_back(f_tree);
    neighbors.push_back(s_tree);
    writeLog("INIT (binary tree, vertex)");
}

Tree::~Tree() {
    neighbors.clear();
}

// Add new vertex to the current Tree.
void Tree::addVert(Point& new_point, double dist) {
    Tree* new_vert = new Tree(new_point, logs_a, dist);
    neighbors.push_back(new_vert);
}

// Getter for number of neighboring vertexes.
int Tree::numTrees() {
    return neighbors.size();
}

//Getter for neighboring vertex.
Tree Tree::operator[](int i) {
    return *(neighbors[i]);
}

// Find vertex with index.
Tree* Tree::findIndex(int i) {
    for (int nei = 0; nei < neighbors.size(); nei++) {
        if ((neighbors[nei])->point.id == i) { return neighbors[nei]; }

        Tree* result = neighbors[nei]->findIndex(i);
        if (result != NULL) { return result; }
    }
    return NULL;
}

// Print tree to the file.
void Tree::displayTree(ofstream& out_f) {
    for (Tree* nei : neighbors) {
        point.print(out_f);
        nei->point.print(out_f);
        out_f << endl << endl;
        nei->displayTree(out_f);
    }
}

// Print Tree as nested list.
void Tree::print(int indent) {
    string cur_indent(indent, '\t');
    cout << cur_indent << point << endl;
    for (Tree* nei : neighbors) {
        nei->print(indent + 1);
    }
}

// Return vector with all edges.
vector<double> Tree::allDist() {
    vector<double> all_dist;
    all_dist.push_back(dist_parent);
    for (Tree* nei : neighbors) {
        vector<double> res = nei->allDist();
        all_dist.insert(all_dist.end(), res.begin(), res.end());
    }
    return all_dist;
}

// Write log-message with date-time note.
void Tree::writeLog(const string& message) {
    logs_a << timeLog() << "TREE: " << message << endl;
}