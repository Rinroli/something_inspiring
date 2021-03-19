// Copyright (c) 2020 Rinroli

#include "KMeans.h"

KMeans::KMeans(int nn, Field& fieldd, ofstream& logs_al)
    : k(nn), field(fieldd), logs_a(logs_al) {
    writeLog("INIT (field)");
    number_poi = field.numPoints();
    for (int ind = 0; ind < k; ind++) {
        int point_ind = rand() % number_poi;
        centers.push_back(field.points[point_ind].getCoord());
        clusters.push_back(vector<int>(number_poi));
    }
    for (int id_point = 0; id_point < number_poi; id_point++) {
        id_points.push_back(id_point);
    }
}

KMeans::KMeans(int nn, Field& fieldd, const vector<int>& id_pointss, ofstream& logs_al)
    : k(nn), field(fieldd), logs_a(logs_al), id_points(id_pointss) {
    writeLog("INIT (cluster)");
    number_poi = id_points.size();
    for (int ind = 0; ind < k; ind++) {
        int point_ind = rand() % number_poi;
        centers.push_back(getCoord(point_ind));
        clusters.push_back(vector<int>(number_poi));
    }
}

// Get point coords by index,
vector<double> KMeans::getCoord(int ind_poi) {
    return (field.getPoint(id_points[ind_poi])).getCoord();
}

// Make clustering using k-means algorithm.
FindClusters KMeans::mainAlgorithm(bool silent) {
    writeLog("Begin mainAlgorithm");
    pointDistribution();
    while (findNewCenters()) {
        pointDistribution();
        step++;
    }
    FindClusters result(logs_a, "KMeans algorithm");
    int ind = 0;
    for (vector<int> cluster : clusters) {
        // for (int a : cluster) {
        //     if (a) {
        //         vector<int> tmp_id_points;
        //         for (int id_poi = 0; id_poi < number_poi; id_poi++) {
        //             if (cluster[id_poi]) { tmp_id_points.push_back(id_poi); }
        //         }
        //         result += Cluster(ind, tmp_id_points, logs_a, &field);
        //         ind++;
        //         break;
        //     }
        // }
        if (any_of(cluster.begin(), cluster.end(), [](int a) {return a;})) {
            vector<int> tmp_id_points;
            for (int id_poi = 0; id_poi < number_poi; id_poi++) {
                if (cluster[id_poi]) { tmp_id_points.push_back(id_poi); }
            }
            result += Cluster(ind, tmp_id_points, logs_a, &field);
            ind++;
        }
    }
    writeLog("\tTurned out " + to_string(result.numClusters()) + " clusters in " + to_string(step) + " steps");
    if (not silent) {
        cout << "Turned out " << to_string(result.numClusters()) << " clusters in " << step << " steps\n";
    }
    return result;
}

// Find new centers as average of the cluster. 
// Return true, if there is any new ones, else false.
bool KMeans::findNewCenters() {
    writeLog("New step");
    bool result = false;
    for (int ind = 0; ind < centers.size(); ind++) {
        vector<double> tmp(2);
        int num_points = 0;
        for (int a_ind = 0; a_ind < number_poi; a_ind++) {
            vector<double> point = getCoord(a_ind);
            int cur_mark = clusters[ind][a_ind];
            tmp[0] += cur_mark * point[0];
            tmp[1] += cur_mark * point[1];
            num_points += cur_mark;
        }
        tmp[0] /= num_points;
        tmp[1] /= num_points;
        if (distPoints(centers[ind], tmp) > EPS) { result = true; }
        centers[ind] = tmp;
    }
    return result;
}

// Find the nearest points to the centers.
void KMeans::pointDistribution() {
    for (int ind = 0; ind < k; ind++) {
        for (int ind_p = 0; ind_p < number_poi; ind_p++) {
            clusters[ind][ind_p] = 0;
        }
    }
    for (int ind_poi = 0; ind_poi < number_poi; ind_poi++) {
        Point point = field.getPoint(id_points[ind_poi]);
        clusters[nearestCenter(point)][ind_poi] = 1;
    }
}

// Find the nearest center to the given point.
int KMeans::nearestCenter(const Point& point) {
    double min_dist = INF;
    int nearest_center = -1;
    for (int ind_center = 0; ind_center < centers.size(); ind_center++) {
        double dist = distPoints(point, centers[ind_center]);
        if (dist < min_dist) {
            min_dist = dist;
            nearest_center = ind_center;
        }
    }
    return nearest_center;
}

// Write log-message with date-time note.
void KMeans::writeLog(const string& message) {
    logs_a << timeLog() << "KMEANS: " << message << endl;
}