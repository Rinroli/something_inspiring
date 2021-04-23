// Copyright (c) 2020 Rinroli

#pragma once

#include <iostream>
#include <vector>

#include "Controller.h"
#include "Cluster.h"
#include "Forel.h"
#include "Field.h"

class Cluster;

using namespace std;

#ifndef FIND_CLUSTERS
#define FIND_CLUSTERS

class FindClusters
{
public:
    FindClusters(ofstream& logs_al, string source, int nu_points);
    ~FindClusters();
    Cluster& operator[](int i);
    void coutInfo(ostream& str_res=cout);
    int numClusters();
    void addDust(int id_point, double indicator=1);
    void setID(int i);
    const string source;
    int id = 0;
    friend void operator+=(FindClusters& left, Cluster new_cl);
    friend class Forel;
    friend class Controller;

private:
    vector<Cluster> clusters;
    vector<double> dust_points;
    int nu_points;
    void writeLog(const string& message);
    ofstream& logs_a;
};

#endif // FIND_CLUSTERS