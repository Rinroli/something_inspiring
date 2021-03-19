// Copyright (c) 2020 Rinroli

#include "EMalgorithm.h"

EMAlgorithm::EMAlgorithm(int kk, Field& fieldd, ofstream& logs_al)
    : k(kk), field(fieldd), logs_a(logs_al) {
    writeLog("INIT");
    size_field = field.numPoints();

    for (int j = 0; j < k; j++) {

        all_sigma.push_back(vector<vector<double>>{ {1, 0}, { 0, 1 } });

        clu_prob.push_back(1. / k);

        point_clu_prob.push_back(vector <double>(size_field));

        all_mu.push_back(vector<double>(2));
        all_mu[j] = field.getPoint(randInt(size_field)).getCoord();
    }
    n_vect = vector<double>(k);
}

// Mahalanobis square distance between a point x_i and a distribution p_j
double EMAlgorithm::distMahalanobis(int p_j, int x_i) {
    vector<double> tmp_point = field.getPoint(x_i).getCoord();
    vector<vector<double>> sigma_j = all_sigma[p_j];
    vector<vector<double>> inv_m{ {sigma_j[1][1], -sigma_j[1][0]}, {-sigma_j[0][1], sigma_j[0][0]} };
    for (int a = 0; a < 2; a++) {
        for (int b = 0; b < 2; b++) {
            inv_m[a][b] /= determinant(sigma_j);
        }
    }
    tmp_point[0] -= all_mu[p_j][0];
    tmp_point[1] -= all_mu[p_j][1];

    vector<double> tmp_dist{ inv_m[0][0] * tmp_point[0] + inv_m[0][1] * tmp_point[1],
                            inv_m[1][0] * tmp_point[0] + inv_m[1][1] * tmp_point[1] };
    return tmp_point[0] * tmp_dist[0] + tmp_point[1] * tmp_dist[1];
}

// Find density distribution of the clusters.
double EMAlgorithm::findPointProb(int p_j, int x_i) {
    double result = 1;
    vector<vector<double>> sigma_j = all_sigma[p_j];
    double delta_sq = distMahalanobis(p_j, x_i);

    result /= 2 * PI * sqrt(determinant(sigma_j));
    result *= exp((-1) * delta_sq / 2);
    return result;
}

// "Expectation" step.
bool EMAlgorithm::stepE() {
    writeLog("Begin step E #" + to_string(step));
    bool the_end = true;
    vector<double> sum_prob;
    for (int i = 0; i < size_field; i++) {
        sum_prob.push_back(0);
        for (int j = 0; j < k; j++) {
            sum_prob[i] += clu_prob[j] * findPointProb(j, i);
        }
    }
    for (int j = 0; j < k; j++) {
        vector<double> old_param = point_clu_prob[j];
        for (int i = 0; i < size_field; i++) {
            point_clu_prob[j][i] = (clu_prob[j] * findPointProb(j, i)) / sum_prob[i];
        }
        if (the_end && distVectors(old_param, point_clu_prob[j]) > EPS) { the_end = false; }
    }
    writeLog("\tEnd step E");
    return the_end;
}

// "Maximization" step.
void EMAlgorithm::stepM() {
    writeLog("Begin step M #" + to_string(step));
    for (int j = 0; j < k; j++) {
        n_vect[j] = 0;
        all_mu[j] = vector<double>{ 0,0 };
        all_sigma[j] = vector<vector<double>>{ {0,0}, {0,0} };
        for (int i = 0; i < size_field; i++) {
            vector<double> cur_point = field.getPoint(i).getCoord();
            double cur_y = point_clu_prob[j][i];
            n_vect[j] += cur_y;  // New n_j

            all_mu[j][0] += cur_y * cur_point[0];  // New mu_j
            all_mu[j][1] += cur_y * cur_point[1];
        }
        all_mu[j][0] /= n_vect[j];
        all_mu[j][1] /= n_vect[j];

        for (int i = 0; i < size_field; i++) {
            vector<double> cur_point = field.getPoint(i).getCoord();
            double cur_y = point_clu_prob[j][i];
            all_sigma[j][0][0] += cur_y * (cur_point[0] - all_mu[j][0]) * (cur_point[0] - all_mu[j][0]);  // New sigma_j
            all_sigma[j][0][1] += cur_y * (cur_point[0] - all_mu[j][0]) * (cur_point[1] - all_mu[j][1]);
            all_sigma[j][1][0] += cur_y * (cur_point[1] - all_mu[j][1]) * (cur_point[0] - all_mu[j][0]);
            all_sigma[j][1][1] += cur_y * (cur_point[1] - all_mu[j][1]) * (cur_point[1] - all_mu[j][1]);
        }
        all_sigma[j][0][0] /= n_vect[j];
        all_sigma[j][1][0] /= n_vect[j];
        all_sigma[j][0][1] /= n_vect[j];
        all_sigma[j][1][1] /= n_vect[j];

        clu_prob[j] = n_vect[j] / size_field;
    }
    writeLog("\tEnd step M");
}

// Save step to the files in directory data/em.
void EMAlgorithm::saveStep() {
    writeLog("SAVE STEP");
    ofstream points_step(field.output_directory + "/em/points_" + to_string(step) + ".plt");
    ofstream ellipses_step(field.output_directory + "/em/ellipse_" + to_string(step) + ".plt");
    vector<vector<int>> clusters = findNearest();
    for (int j = 0; j < k; j++) {
        vector<int> cluster = clusters[j];
        for (int point_id : cluster) {
            field.getPoint(point_id).print(points_step);
        }
        points_step << endl << endl;

        vector<vector<double>> eigen_data = eigen(all_sigma[j]);
        int ind_max = (eigen_data[2][0] > eigen_data[2][1]) ? 0 : 1;
        if (eigen_data[ind_max][1] < 0) {
            eigen_data[ind_max][0] *= -1;
            eigen_data[ind_max][1] *= -1;
        }
        double angle = acos(eigen_data[ind_max][0] / sqrt(eigen_data[ind_max][0] * eigen_data[ind_max][0] +
            eigen_data[ind_max][1] * eigen_data[ind_max][1]));
        angle *= 180;
        angle /= PI;
        ellipses_step << all_mu[j][0] << " " << all_mu[j][1] << " " <<
            20 * eigen_data[2][1 - ind_max] << " " << 20 * eigen_data[2][ind_max] <<
            " " << angle << endl;
    }

    points_step.close();
    ellipses_step.close();
}

// Find points thst fit probality.
vector<vector<int>> EMAlgorithm::findNearest() {
    vector<vector<int>> clusters(k);
    for (int ind_point = 0; ind_point < size_field; ind_point++) {
        double max_prob = 0;
        int max_clu = -1;
        for (int p_j = 0; p_j < k; p_j++) {
            if (point_clu_prob[p_j][ind_point] > max_prob) {
                max_prob = point_clu_prob[p_j][ind_point];
                max_clu = p_j;
            }
        }
        clusters[max_clu].push_back(ind_point);
    }
    return clusters;
}

// Find clusters by em-algorithm
FindClusters EMAlgorithm::mainAlgorithm() {
    FindClusters result(logs_a, "EM-algorithm; k = " + to_string(k));
    bool it_is_the_end = false;
    while (not it_is_the_end) {
        it_is_the_end = stepE();
        stepM();
        saveStep();
        writeLog("STEP #" + to_string(step));
        step++;
    }
    vector<vector<int>> clusters = findNearest();
    for (int id_clu = 0; id_clu < k; id_clu++) {
        result += Cluster(id_clu, clusters[id_clu], logs_a, &field);
    }

    writeLog("\tTurned out " + to_string(result.numClusters()) + " clusters in " + to_string(step) + " steps");
    cout << "Turned out " << to_string(result.numClusters()) << " clusters in " << step << " steps\n";

    string cur_template = (field.if_test) ? "test_" : "";
    ifstream anim_templ("data/templates/" + cur_template + "em_animate.template");
    ofstream anim(field.output_directory + "/gnu_em_animate.plt");

    string new_line;
    while (!anim_templ.eof()) {
        getline(anim_templ, new_line);
        anim << new_line << endl;
    }
    anim << "do for [i=0:" << to_string(step - 1) << "] {" << endl << "\tplot ";
    for (int ind_k = 0; ind_k < k; ind_k++) {
        anim << "\t\t'" << field.output_directory << "/em/points_'.i.'.plt' index " << to_string(ind_k) <<
            " w p title \"#" << to_string(ind_k) << "\",\\" << endl;
    }
    anim << "\t\t'" << field.output_directory << "/em/ellipse_'.i.'.plt' using 1:2:3:4:5 with ellipses lc rgb \"red\" title \"ellipses\"\n}";

    return result;
}

// Write log-message with date-time note.
void EMAlgorithm::writeLog(const string& message) {
    logs_a << timeLog() << "EMALGORITHM: " << message << endl;
}
