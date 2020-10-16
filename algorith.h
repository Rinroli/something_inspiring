// Copyright (c) 2020 Rinroli

#include "func_file.h"
// #include <string>
#include <vector>
#include <fstream>

using namespace std;

#ifndef WAVE_CLUSTERS
#define WAVE_CLUSTERS

class WaveClusters
{
public:
    WaveClusters(BinMatrix matrix, ofstream &logs_field,
                 Field *field);
    ~WaveClusters();
    FindClusters mainAlgorithm();

private:
    void printEdges();
    bool stepWave();
    void checkNeighbors(int i);
    bool findNextBegin();
    void writeLog(string message);
    vector<int> marks, step_points, sec_step_points;
    BinMatrix matrix_inc;
    int cur_step;
    ofstream &logs_a;
    Field *p_field;
};

#endif // WAVE_CLUSTERS
