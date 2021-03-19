// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>

#include "Field.h" // WARNING
#include "FindClusters.h"
#include "my_functions.h"

class FindClusters;

class Field;

#define EPS 0.001

using namespace std;

#if !defined(EMALGORITHM)
#define EMALGORITHM

class EMAlgorithm
{
public:
    EMAlgorithm(int k, Field& field, ofstream& logs_al);
    ~EMAlgorithm() {}
    FindClusters mainAlgorithm();
private:
    vector<vector<double>> point_clu_prob;
    vector<double> clu_prob, n_vect;
    vector<vector<double>> all_mu;
    vector<vector<vector<double>>> all_sigma;
    int k, step = 0, size_field;
    Field& field;
    ofstream& logs_a;
    double findPointProb(int p_j, int x_i);
    vector<vector<int>> findNearest();
    bool stepE();
    void stepM();
    double distMahalanobis(int p_j, int x_i);
    void saveStep();
    void writeLog(const string& message);
};

#endif // EMALGORITHM