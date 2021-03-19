// Copyright (c) 2020 Rinroli

#include "KerKMeans.h"

KerKMeans::KerKMeans(int kk, int pp, Field& fieldd, ofstream& logs_al)
    : k(kk), p(pp), logs_a(logs_al), field(fieldd) {
    writeLog("INIT");
    number_poi = field.numPoints();
    clusters.resize(k);
    centers.resize(k);
    KMeans tmp_kmeans(k, field, logs_a);
    tmp_kmeans.mainAlgorithm(true);
    for (int ind_center = 0; ind_center < k; ind_center++) {
        centers[ind_center].push_back(tmp_kmeans.centers[ind_center]);
    }
}

// Main loop for clustering.
FindClusters KerKMeans::mainAlgorithm() {
    writeLog("Begin mainAlgorithm");
    pointDistribution();
    saveStep();
    newKernels();
    bool is_stable = false;
    for (int i = 0; i < LIMIT; i++) {
        step++;
        if (pointDistribution()) {
            saveStep();
            is_stable = true;
            break;
        }
        saveStep();
        newKernels();
    }

    FindClusters result(logs_a, "KerKMeans algorithm (" + to_string(is_stable) + ")");
    int ind = 0;
    for (vector<int> cluster : clusters) {
        result += Cluster(ind, cluster, logs_a, &field);
        ind++;
    }
    string cur_template = (field.if_test) ? "test_" : "";
    ifstream anim_templ("data/templates/" + cur_template + "ker_animate.template");
    string cur_name = (field.if_test) ? field.output_name : "ker";
    ofstream anim(field.output_directory + "/gnu_" + cur_name + "_animate.plt");
    anim.setf(ios::boolalpha);

    string new_line;
    while (!anim_templ.eof()) {
        getline(anim_templ, new_line);
        anim << new_line << endl;
    }
    anim << "set title \"KMeans-algorithm with kernels (stable - " <<
        is_stable << ", steps " << step << ")" << endl;
    anim << "do for [i=0:" << to_string(step) << "] {" << endl << "\tplot ";
    for (int ind_k = 0; ind_k < k; ind_k++) {
        anim << "\t\t'" << field.output_directory << "/ker/points_'.i.'.plt' index " << to_string(ind_k) <<
            " w p title \"#" << to_string(ind_k) << "\",\\" << endl;
    }
    anim << "\t\t'" << field.output_directory << "/ker/kernel_'.i.'.plt' w p lc rgb \"red\" title \"kernels\"" <<
        endl << "}" << endl;
    writeLog("\tTurned out " + to_string(result.numClusters()) + " clusters in " + to_string(step) + " steps");
    cout << "Turned out " << to_string(result.numClusters()) << " clusters in " << step << " steps\n";
    return result;
}

// Find the nearest kernels to the centers.
bool KerKMeans::pointDistribution() {
    vector<vector<int>> new_clusters = clusters;
    clusters.clear();
    clusters.resize(k);
    for (int ind_poi = 0; ind_poi < number_poi; ind_poi++) {
        Point point = field.getPoint(ind_poi);
        clusters[nearestCenter(point)].push_back(ind_poi);
    }
    for (int clu = 0; clu < k; clu++) {
        if (clusters[clu].size() != new_clusters[clu].size()) { return false; }
        for (int poi = 0; poi < clusters[clu].size(); poi++) {
            if (clusters[clu][poi] != new_clusters[clu][poi]) { return false; }
        }
    }
    return true;
}

// Find the nearest center to the given point.
int KerKMeans::nearestCenter(const Point& point) {
    double min_dist = INF;
    int nearest_center = -1;
    for (int ind_group = 0; ind_group < k; ind_group++) {
        for (int ind_cent = 0; ind_cent < centers[ind_group].size(); ind_cent++) {
            vector<double> cent = centers[ind_group][ind_cent];
            double dist = distPoints(point, cent);
            if (dist < min_dist) {
                min_dist = dist;
                nearest_center = ind_group;
            }
        }
    }
    return nearest_center;
}

// Find new kernels for group.
void KerKMeans::newKernels() {
    for (int ind_group = 0; ind_group < k; ind_group++) {
        KMeans groupKM(p, field, clusters[ind_group], logs_a);
        groupKM.mainAlgorithm(true);
        centers[ind_group] = groupKM.centers;
    }
}

// Save step to the files in directory data/em.
void KerKMeans::saveStep() {
    writeLog("SAVE STEP");
    ofstream points_step(field.output_directory + "/ker/points_" + to_string(step) + ".plt");
    ofstream kernels_step(field.output_directory + "/ker/kernel_" + to_string(step) + ".plt");
    for (int j = 0; j < k; j++) {
        for (int point_id : clusters[j]) {
            field.getPoint(point_id).print(points_step);
        }
        points_step << endl << endl;
        for (vector<double> kernel : centers[j]) {
            kernels_step << kernel[0] << " " << kernel[1] << endl;
        }
    }

    points_step.close();
    kernels_step.close();
}

// Write log-message with date-time note.
void KerKMeans::writeLog(const string& message) {
    logs_a << timeLog() << "KERKMEANS: " << message << endl;
}
