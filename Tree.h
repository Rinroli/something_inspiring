// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>

#include "Point.h"
#include "my_functions.h"

class Point;

using namespace std;

#if !defined(TREE)
#define TREE

class Tree {
public:
    Tree(Point& point, ofstream& logs_al, double dist = 0);
    Tree(Point& point, Tree* f_tree, Tree* s_tree, ofstream& logs_al);
    ~Tree();
    void addVert(Point& point, double dist);
    void print(int indent = 0);
    int numTrees();
    vector<double> allDist();
    Tree operator[](int i);
    Tree* findIndex(int i);
    void displayTree(ofstream& out_f);

private:
    Point& point;
    double dist_parent = -1;
    vector<Tree*> neighbors;
    void writeLog(const string& message);
    ofstream& logs_a;
};

#endif // TREE