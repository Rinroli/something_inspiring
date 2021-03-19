// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>

#include "Field.h"
#include "FindClusters.h"
#include "Tree.h"
#include "Cluster.h"

class Field;
class Cluster;

using namespace std;

#if !defined(HIERARCH)
#define HIERARCH

class Hierarch
{
public:
    Hierarch(int k, Field* p_field, ofstream& ofstream);
    ~Hierarch();
    FindClusters mainAlgorithm();
private:
    vector<Tree*> current_trees;
    // vector<vector<int>> current_clusters;
    vector<Cluster> current_clusters;
    vector<vector<double>> dist_matrix;
    int k, num_clusters, step = 0, frame = 0;
    Field* p_field;
    ofstream& logs;
    vector<int> findMinDist();
    vector<int> recountDistMatrix(const vector<int>& find_ind);
    double getDist(int ind_1, int ind_2);
    void newFrame();
    void saveAnimation();
    void writeLog(const string& message);
};

#endif // HIERARCH