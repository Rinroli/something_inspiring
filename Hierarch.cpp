// Copyright (c) 2020 Rinroli

#include "Hierarch.h"

Hierarch::Hierarch(int kk, Field* p_fieldd, ofstream& logs_a)
    : k(kk), logs(logs_a), p_field(p_fieldd) {
    writeLog("ININ");
    num_clusters = p_field->numPoints();
    for (int ind_poi = 0; ind_poi < num_clusters; ind_poi++) {
        Tree* tmp_tree = new Tree(p_field->getPoint(ind_poi), logs);
        current_trees.push_back(tmp_tree);

        Cluster* tmp = new Cluster(ind_poi, vector<int> {ind_poi}, logs, p_field);
        current_clusters.push_back(*tmp);
    }
    dist_matrix.resize(num_clusters);
    for (int ind = 0; ind < num_clusters; ind++) {
        dist_matrix[ind].resize(num_clusters);
    }

    // First distance matrix init
    for (int ind_poi = 0; ind_poi < num_clusters; ind_poi++) {
        for (int sec_poi = ind_poi; sec_poi < num_clusters; sec_poi++) {
            double tmp_dist = p_field->getDist(ind_poi, sec_poi);

            dist_matrix[ind_poi][sec_poi] = tmp_dist;
            dist_matrix[sec_poi][ind_poi] = tmp_dist;
        }
    }
    writeLog("Initialization done");
}

Hierarch::~Hierarch() {
    current_trees.clear();
}

// Call algorithm.
FindClusters Hierarch::mainAlgorithm() {
    writeLog("Start algorithm");
    FindClusters result(logs, "Hierarch", p_field->numPoints());
    while (num_clusters > k) {
        writeLog("STEP #" + to_string(step));
        vector<int> find_ind = recountDistMatrix(findMinDist());

        Point tmp(logs);
        Tree* tmp_tree = new Tree(tmp, current_trees[find_ind[0]], current_trees[find_ind[1]], logs);
        current_trees.erase(current_trees.begin() + find_ind[1]);
        current_trees[find_ind[0]] = tmp_tree;
    
        current_clusters[find_ind[0]] += current_clusters[find_ind[1]];
        current_clusters.erase(current_clusters.begin() + find_ind[1]);
        num_clusters--;
        step++;
        newFrame();
    }
    for (int ind_clu = 0; ind_clu < k; ind_clu++) {
        result += current_clusters[ind_clu];
    }

    saveAnimation();
    return result;
}

// Find trees with min distance between
vector<int> Hierarch::findMinDist() {
    writeLog("FINDMINDIST");
    double min_dist = INF;
    vector<int> min_ind{ -1, -1 };
    for (int ind = 0; ind < num_clusters; ind++) {
        for (int sec_ind = ind + 1; sec_ind < num_clusters; sec_ind++) {
            double tmp_dist = getDist(ind, sec_ind);
            if (tmp_dist < min_dist) {
                min_dist = tmp_dist;
                min_ind = vector<int>{ ind, sec_ind };
            }
        }
    }
    return min_ind;
}

// Find complicated distance between clusters
double Hierarch::getDist(int ind, int sec_ind) {
    double sum_dist = 0;
    for (int id_f : current_clusters[ind].getPoints()) {
        for (int id_s : current_clusters[sec_ind].getPoints()) {
            sum_dist += p_field->getDist(id_f, id_s);
        }
    }
    sum_dist /= (current_clusters[ind].numPoints() * current_clusters[sec_ind].numPoints());
    return sum_dist;
}

// Recount distance matrix.
vector<int> Hierarch::recountDistMatrix(const vector<int>& find_ind) {
    writeLog("RECOUNT");
    int min_i = (find_ind[0] < find_ind[1]) ? find_ind[0] : find_ind[1];
    int max_i = (find_ind[0] > find_ind[1]) ? find_ind[0] : find_ind[1];

    int min_size = current_clusters[min_i].numPoints();
    int max_size = current_clusters[max_i].numPoints();

    for (int ind_other = 0; ind_other < num_clusters; ind_other++) {
        if (ind_other != min_i && ind_other != max_i) {
            double tmp_dist = (dist_matrix[max_i][ind_other] / min_size);

            dist_matrix[min_i][ind_other] /= max_size;
            dist_matrix[ind_other][min_i] /= max_size;

            dist_matrix[min_i][ind_other] += tmp_dist;
            dist_matrix[ind_other][min_i] += tmp_dist;
        }
    }

    dist_matrix.erase(dist_matrix.begin() + max_i);
    for (int ind = 0; ind < dist_matrix.size(); ind++) {
        dist_matrix[ind].erase(dist_matrix[ind].begin() + max_i);
    }

    return vector<int>{min_i, max_i};
}

// Save command file for animation for gnuplot.
void Hierarch::saveAnimation() {
    string cur_template = (p_field->if_test) ? "test_" : "";
    ifstream anim_templ("data/templates/" + cur_template + "hierarch_animate.template");
    ofstream anim(p_field->output_directory + "/gnu_hierarch_animate.plt");

    string new_line;
    while (!anim_templ.eof()) {
        getline(anim_templ, new_line);
        anim << new_line << endl;
    }
    anim << "do for [i=0:" << to_string(frame - 1) << "] {" << endl << "\tplot ";
    for (int ind_k = 0; ind_k < p_field->numPoints(); ind_k++) {
        anim << "\t\t'" << p_field->output_directory << "/hierarch/frame_'.i.'.plt' index " << to_string(ind_k) <<
            " w p title \"#" << to_string(ind_k) << "\",\\" << endl;
    }
    anim << "}" << endl;
}

// Create new frame at 'data/hierarch/frame_i.plt'
void Hierarch::newFrame() {
    writeLog("New frame");
    ofstream file_clu(p_field->output_directory + "/hierarch/frame_" + to_string(frame) + ".plt");

    for (int ind_clu = 0; ind_clu < current_clusters.size(); ind_clu++) {
            current_clusters[ind_clu].printGnu(file_clu);
            file_clu << endl << endl;
    }

    file_clu.close();
    frame++;
}

// Write log-message with date-time note.
void Hierarch::writeLog(const string& message) {
    logs << timeLog() << "HIERARCH: " << message << endl;
}