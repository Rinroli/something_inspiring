// Copyright (c) 2020 Rinroli

#include "FindClusters.h"

FindClusters::FindClusters(ofstream& logs_al, const string sourcee)
    : logs_a(logs_al), source(sourcee)
{
    writeLog("INIT");
}

FindClusters::~FindClusters()
{
    clusters.clear();
    writeLog("DELETE");
}

// Getter for the cluster from vector.
Cluster& FindClusters::operator[](int i) {
    return clusters[i];
}

// Setter for id.
void FindClusters::setID(int i) {
    id = i;
}

// Cout info about FindClusters to the standart output.
void FindClusters::coutInfo() {
    cout << "\t#" << id << " Source: " << source << ", "
        << clusters.size() << " clusters." << endl;
    for (Cluster cl : clusters) {
        cl.coutInfo();
    }
}

// Simple addition of new cluster to FindClusters
void operator+=(FindClusters& left, Cluster new_cluster) {
    left.clusters.push_back(new_cluster);
}

// Return number of clusters.
int FindClusters::numClusters() {
    return clusters.size();
}

// Write log-message with date-time note.
void FindClusters::writeLog(const string& message) {
    logs_a << timeLog() << "FIND_CLUSTERS: " << message << endl;
}