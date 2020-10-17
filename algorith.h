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
    WaveClusters(const BinMatrix &matrix, ofstream &logs_field,
                 Field *field);
    ~WaveClusters();
    FindClusters mainAlgorithm();

private:
    bool stepWave();
    void checkNeighbors(int i);
    bool findNextBegin();
    void writeLog(const string &message);
    vector<int> marks, step_points, sec_step_points;
    BinMatrix matrix_inc;
    int cur_step = 0;
    ofstream &logs_a;
    Field *p_field;
};

#endif // WAVE_CLUSTERS
