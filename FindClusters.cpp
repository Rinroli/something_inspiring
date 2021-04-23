// Copyright (c) 2020 Rinroli

#include "FindClusters.h"

FindClusters::FindClusters(ofstream& logs_al, const string sourcee, int nu_pointss)
    : logs_a(logs_al), source(sourcee), nu_points(nu_pointss)
{
    dust_points.resize(nu_points);
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
void FindClusters::coutInfo(ostream& str_to_cli) {
    str_to_cli << "\t#" << id << " Source: " << source << ", "
        << clusters.size() << " clusters." << endl;
    for (Cluster cl : clusters) {
        cl.coutInfo(str_to_cli);
    }
}

// Add point by id to dust vector
void FindClusters::addDust(int id_point, double indicator) {
    dust_points[id_point] = indicator;
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