// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>

#include "Field.h"  // WARNING
#include "Cluster.h"
#include "Delaunay.h"
#include "my_functions.h"
#include "EMalgorithm.h"
#include "FindClusters.h"
#include "KMeans.h"
#include "KerKMeans.h"
#include "WaveClusters.h"
#include "Forel.h"
#include "Hierarch.h"

class Field;

using namespace std;

#ifndef CONTROLLER
#define CONTROLLER

class Controller
{
public:
    Controller(vector<bool> if_logs, vector<string> name_logs);
    ~Controller();
    bool showHelp();
    bool showInfoField();
    bool showInfoFClusters();
    bool showBuffer();
    bool addToBuffer(int ind);
    bool putBuffer();
    bool emptyBuffer();
    bool rotateBuffer(double alpha);
    bool moveBuffer(double x, double y);
    bool zoomBuffer(double k);
    bool printField(bool clouds=true, int i=0);
    bool saveHist();
    bool saveHist(Cluster cluster);
    bool streeHist();
    bool findR();
    bool kMeans(int n);
    bool kerKMeans(int k, int p);
    bool eMAlgorithm(int n);
    bool forelAlg(double R);
    bool hierarchClustering(int k);
    bool enterAnalysis();
    bool minSpanTree();
    bool delaunayTriangulation();
    bool createIncMatrix(double delta);
    bool createDBMatrix(double delta, int k);
    bool preHist(vector<string> args);
    bool genCloud(double mX, double mY, double sX, double sY, int nu_points);
    bool waveClusters(int i);
    bool displayGraph(int i);
    void beginTest(const string& output_dir, const string& output_na,
        const string& gen_f = "none");
    void writeLog(const string& message);

private:
    Field* field;
    ofstream logs;
    bool if_test = false;
    string output_directory = "data";
    string output_name;
    string gen_file;
};

#endif // CONTROLLER