// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>

#include "Field.h"
#include "BinMatrix.h"  // WARNING!
#include "FindClusters.h"

class Field;

using namespace std;

#ifndef WAVE_CLUSTERS
#define WAVE_CLUSTERS

class WaveClusters
{
public:
    WaveClusters(const BinMatrix& matrix, ofstream& logs_field,
        Field* field);
    ~WaveClusters();
    FindClusters mainAlgorithm();

private:
    bool stepWave();
    void checkNeighbors(int i);
    bool findNextBegin();
    void writeLog(const string& message);
    vector<int> marks, step_points, sec_step_points;
    BinMatrix matrix_inc;
    int cur_step = 0;
    ofstream& logs_a;
    Field* p_field;
};

#endif // WAVE_CLUSTERS