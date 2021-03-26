// Copyright (c) 2020 Rinroli

#include "WaveClusters.h"

WaveClusters::WaveClusters(const BinMatrix& matrix, ofstream& logs_field,
    Field* field)
    : logs_a(logs_field), matrix_inc(matrix)
{
    p_field = field;
    marks = matrix_inc.marks;

    findNextBegin();
    writeLog("INIT");
}

WaveClusters::~WaveClusters()
{
    writeLog("DELETE");
}

// One step of "burning" points.
bool WaveClusters::stepWave() {
    cur_step++;
    for (int i = 0; i < step_points.size(); ++i) {
        checkNeighbors(step_points[i]);
    }

    if (sec_step_points.size() == 0) {
        step_points.clear();
        return false;
    }
    step_points.swap(sec_step_points);
    sec_step_points.clear();
    return true;
}

// Check if neighbors can be marked.
void WaveClusters::checkNeighbors(int i) {
    for (int ind_poi = 0; ind_poi < matrix_inc.size; ++ind_poi) {
        if (matrix_inc[i][ind_poi]) {
            if (not marks[ind_poi]) {
                marks[ind_poi] = cur_step;
                p_field->setAPoint(ind_poi, cur_step);
                sec_step_points.push_back(ind_poi);
            }
        }
    }
}

// Find begining of the next cluster.
bool WaveClusters::findNextBegin() {
    writeLog("Begin findNextBegin");
    for (int i = 0; i < marks.size(); ++i) {
        if (marks[i] == 0) {
            marks[i] = 1;
            step_points.push_back(i);
            return true;
        }
    }
    return false;
}

// Create clusters by wave (dbscan) algorithm.
FindClusters WaveClusters::mainAlgorithm()
{
    writeLog("BEGIN CLUSTERING");
    string name_wave = "Wave clustering (delta = " + to_string(matrix_inc.delta) + ")";
    string name_dbscan = "DBSCAN (k=" + to_string(matrix_inc.k) + 
        ", delta = " + to_string(matrix_inc.delta) + ")";
    FindClusters result_vector(logs_a, (matrix_inc.k == 1) ? name_wave : name_dbscan,
        p_field->numPoints());
    int ind = 0;
    for (int dust_p = 0; dust_p < marks.size(); dust_p++) {
        if (marks[dust_p] == -1) {
            result_vector.addDust(dust_p);
        }
    }
    while (findNextBegin()) {
        cur_step = 1;
        while (stepWave()) {}
        Cluster new_cluster(ind, logs_a, p_field);
        for (int i = 0; i < marks.size(); ++i) {
            if (marks[i] > 0) {
                new_cluster += i;
                marks[i] = -1;
            }
        }
        result_vector += new_cluster;
        ind++;
    }
    writeLog("\tTurned out " + to_string(result_vector.numClusters()) + " clusters");
    cout << "Turned out " << to_string(result_vector.numClusters()) << " clusters. ";
    return result_vector;
}

// Write log-message with date-time note. 
void WaveClusters::writeLog(const string& message)
{
    logs_a << timeLog() << "WAVE: " << message << endl;
}
