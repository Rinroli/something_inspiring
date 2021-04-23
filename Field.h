// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Point.h"
#include "Cluster.h"
// #include "Cloud.h"
#include "BinMatrix.h"
#include "Tree.h"
#include "Triangulation.h"
#include "Buffer.h"
#include "FindClusters.h"

class Tree;
class Triangulation;
class Buffer;
class Cluster;
class Cloud;
class FindClusters;

using namespace std;

#ifndef FIELD
#define FIELD

class Field
{
public:
    Field(vector<bool> if_logs, vector<string> name_logs, stringstream& message);
    ~Field();
    bool createCloud(double mX, double mY,
        double sX, double sY, int nu_points);
    bool createCloud(vector<Point> points);
    void print(ofstream& out_f);
    void print(int i, ofstream& out_f);
    int numClouds();
    int numFClusters();
    int numPoints();
    double getDist(int ind1, int ind2);
    double getDist(Point point1, int ind2);
    vector<double> lineThroughPoints(int poi_1, int poi_2);
    vector<double> lineThroughPoints(vector<double> vpoi_1, int poi_2);
    Point& operator[](int i);
    FindClusters& getFCluster(int i);
    BinMatrix& getBinMatrix(int i);
    Point& getPoint(int i);
    Cloud& getCloud(int i);
    vector<double> getBox();

    bool addToBuffer(int i);
    bool putBuffer();
    bool emptyBuffer();
    bool rotateBuffer(double alpha);
    bool moveBuffer(double x, double y);
    bool zoomBuffer(double k);
    void showBuffer();

    int numBinMatrix();
    bool ifReadonly();
    bool enterAnalysis();
    void minSpanTree();
    void delaunayTriangulation();
    void binDBMatrix(double delta, int k);
    void binMatrix(double delta);
    void setAPoint(int i, int value);
    void addFC(FindClusters new_fc);
    void drawBinGraph(int i);
    void beginTest(const string& output_dir, const string& output_na);
    vector<Point> points;
    vector<vector<double>> matrix;
    ofstream logs, logs_a;
    bool incMatrix(int k = 1);
    bool if_test = false;
    string output_name = "none";
    string output_directory = "data";
    friend class Controller;
private:
    vector<FindClusters> fclusters;
    vector<Cloud> clouds;
    Buffer* buffer;
    void resizeAllClouds();
    void writeLog(const string& message);
    vector<double> containing_box{ 0, 0, 0, 0 };  // {x_1, x_2, y_1, y_2}
    bool readonly;
    void updateD();
    void findBox();
    vector<BinMatrix> bin_matrixes;
    Tree* p_tree = nullptr;
    Triangulation* p_triangulation = nullptr;

    stringstream& message;
};

#endif // FIELD