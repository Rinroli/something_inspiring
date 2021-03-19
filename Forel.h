// Copyright (c) 2020 Rinroli

#pragma once

#include <iostream>
#include <vector>

#include "Point.h"
#include "FindClusters.h"
#include "Cluster.h"
#include "Field.h"

class Field;
class Point;
class FindClusters;
class Cluster;

#define EPS 0.001

using namespace std;

#if !defined(FOREL)
#define FOREL

class Forel
{
public:
    Forel(double R, const vector<Point>& real_pointss, Field* p_field,
        ofstream& logs_al, int g_stepp, int frame, int max_clusterss);
    ~Forel();
    vector<FindClusters> mainAlgorithm();
private:
    double R;
    vector<int> points;
    vector<Point> real_points;
    vector<Point> centroids;
    FindClusters* f_clusters = nullptr;
    vector<FindClusters> exported_points;
    Forel* centroid_clustering = nullptr;
    int step = 0, frame, global_step, max_clusters;
    Field* p_field;
    ofstream& logs_a;
    Cluster oneNewCluster();
    Cluster findNeighbourhood(const vector<double>& center);
    void removePoint(int i);
    Point& getPoint(int i);
    void exportPoints();
    void saveAnimation();
    void newFrame();
    void writeLog(const string& message);
};

#endif // FOREL