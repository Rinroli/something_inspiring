// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "Field.h"
#include "FindClusters.h"
#include "Point.h"
#include "KerKMeans.h"

class Field;

#define EPS 0.001

using namespace std;

#if !defined(KMEANS)
#define KMEANS

class KMeans
{
public:
    KMeans(int k, Field& field, ofstream& logs_al);
    KMeans(int k, Field& field, const vector<int>& id_points, ofstream& logs_al);
    ~KMeans() {}
    FindClusters mainAlgorithm(bool silent = false);
    friend class KerKMeans;
private:
    int k, number_poi, step = 1;
    Field& field;
    ofstream& logs_a;
    vector<vector<int>> clusters;
    void pointDistribution();
    int nearestCenter(const Point& point);
    bool findNewCenters();
    vector<double> getCoord(int ind_poi);
    void writeLog(const string& message);
    vector<vector<double>> centers;
    vector<int> id_points;
};

#endif // KMEANS