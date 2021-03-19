// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>

#include "Field.h"
#include "Point.h"
#include "FindClusters.h"
#include "KMeans.h"

class Field;
class FindClusters;

#define LIMIT 250

using namespace std;

#if !defined(KERKMEANS)
#define KERKMEANS

class KerKMeans
{
public:
    KerKMeans(int k, int p, Field& field, ofstream& logs_al);
    ~KerKMeans() {}
    FindClusters mainAlgorithm();
private:
    int p, k, number_poi, step = 0;
    Field& field;
    vector<vector<int>> clusters;
    vector<vector<vector<double>>> centers;
    bool pointDistribution();
    void newKernels();
    void saveStep();
    int nearestCenter(const Point& point);
    ofstream& logs_a;
    void writeLog(const string& message);
};

#endif // KERKMEANS