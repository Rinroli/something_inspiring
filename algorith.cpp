// Copyright (c) 2020 Rinroli

#include "func_file.h"
#include <queue>
#include <cmath>
#include <algorithm>
// #include <string>
#include <vector>


using namespace std;

#define EPS 0.001
#define PI 3.14159265
#define LIMIT 250

class BinMatrix;

///// WAVE_CLUSTERS /////

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
    string name_dbscan = "DBSCAN (k=" + to_string(matrix_inc.k) + ", delta = " + to_string(matrix_inc.delta) + ")";
    FindClusters result_vector(logs_a, (matrix_inc.k == 1) ? name_wave : name_dbscan);
    int ind = 0;
    for (int dust_p = 0; dust_p < marks.size(); dust_p++) {
        if (marks[dust_p] == -1) {
            Cluster new_cluster(ind, logs_a, p_field);
            new_cluster += dust_p;
            ind++;
            result_vector += new_cluster;
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

///// Tree /////

Tree::Tree(Point& pointt, ofstream& logs_al, double dist)
    : point(pointt), logs_a(logs_al), dist_parent(dist) {
    writeLog("INIT (of point #" + to_string(point.id) + ")");
}

Tree::~Tree() {
    neighbors.clear();
}

// Add new vertex to the current Tree.
void Tree::addVert(Point& new_point, double dist) {
    Tree new_vert(new_point, logs_a, dist);
    neighbors.push_back(new_vert);
}

// Getter for number of neighboring vertexes.
int Tree::numTrees() {
    return neighbors.size();
}

//Getter for neighboring vertex.
Tree Tree::operator[](int i) {
    return neighbors[i];
}

// Find vertex with index.
Tree* Tree::findIndex(int i) {
    for (int nei = 0; nei < neighbors.size(); nei++) {
        if (neighbors[nei].point.id == i) { return &neighbors[nei]; }

        Tree* result = neighbors[nei].findIndex(i);
        if (result != NULL) { return result; }
    }
    return NULL;
}

// Print tree to the file.
void Tree::displayTree(ofstream& out_f) {
    for (Tree nei : neighbors) {
        point.print(out_f);
        nei.point.print(out_f);
        out_f << endl << endl;
        nei.displayTree(out_f);
    }
}

// Print Tree as nested list.
void Tree::print(int indent) {
    string cur_indent(indent, '\t');
    cout << cur_indent << point << endl;
    for (Tree nei : neighbors) {
        nei.print(indent + 1);
    }
}

// Return vector with all edges.
vector<double> Tree::allDist() {
    vector<double> all_dist;
    all_dist.push_back(dist_parent);
    for (Tree nei : neighbors) {
        vector<double> res = nei.allDist();
        all_dist.insert(all_dist.end(), res.begin(), res.end());
    }
    return all_dist;
}

// Write log-message with date-time note.
void Tree::writeLog(const string& message) {
    logs_a << timeLog() << "TREE: " << message << endl;
}

///// KMEANS /////

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
        for (int a : cluster) {
            if (a) {
                vector<int> tmp_id_points;
                for (int id_poi = 0; id_poi < number_poi; id_poi++) {
                    if (cluster[id_poi]) { tmp_id_points.push_back(id_poi); }
                }
                result += Cluster(ind, tmp_id_points, logs_a, &field);
                ind++;
                break;
            }
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


///// KERKMEANS /////

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
    ifstream anim_templ("data/templates/ker_animate.template");
    ofstream anim("data/gnu_ker_animate.plt");
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
        anim << "\t\t'data/ker/points_'.i.'.plt' index " << to_string(ind_k) <<
            " w p title \"#" << to_string(ind_k) << "\",\\" << endl;
    }
    anim << "\t\t'data/ker/kernel_'.i.'.plt' w p lc rgb \"red\" title \"kernels\"" <<
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
    ofstream points_step("data/ker/points_" + to_string(step) + ".plt");
    ofstream kernels_step("data/ker/kernel_" + to_string(step) + ".plt");
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


///// EMALGORITHM /////

EMAlgorithm::EMAlgorithm(int kk, Field& fieldd, ofstream& logs_al)
    : k(kk), field(fieldd), logs_a(logs_al) {
    writeLog("INIT");
    size_field = field.numPoints();

    for (int j = 0; j < k; j++) {

        all_sigma.push_back(vector<vector<double>>{ {1, 0}, {0, 1} });

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
    vector<vector<double>> inv_m{{sigma_j[1][1], -sigma_j[1][0]}, {-sigma_j[0][1], sigma_j[0][0]}};
    for (int a = 0; a < 2; a++) {
        for (int b = 0; b < 2; b++) {
            inv_m[a][b] /= determinant(sigma_j);
        }
    }
    tmp_point[0] -= all_mu[p_j][0];
    tmp_point[1] -= all_mu[p_j][1];

    vector<double> tmp_dist{inv_m[0][0] * tmp_point[0] + inv_m[0][1] * tmp_point[1],
                            inv_m[1][0] * tmp_point[0] + inv_m[1][1] * tmp_point[1]};
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
    for (int  j = 0; j < k; j++) {
        n_vect[j] = 0;
        all_mu[j] = vector<double>{0,0};
        all_sigma[j] = vector<vector<double>>{{0,0}, {0,0}};
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
    ofstream points_step("data/em/points_" + to_string(step) + ".plt");
    ofstream ellipses_step("data/em/ellipse_" + to_string(step) + ".plt");
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

    ifstream anim_templ("data/templates/em_animate.template");
    ofstream anim("data/gnu_em_animate.plt");

    string new_line;
    while (!anim_templ.eof()) {
        getline(anim_templ, new_line);
        anim << new_line << endl;
    }
    anim << "do for [i=0:" << to_string(step - 1) << "] {" << endl << "\tplot ";
    for (int ind_k = 0; ind_k < k; ind_k++) {
        anim << "\t\t'data/em/points_'.i.'.plt' index " << to_string(ind_k) <<
                " w p title \"#" << to_string(ind_k) << "\",\\" << endl; 
    }
    anim << "\t\t'data/em/ellipse_'.i.'.plt' using 1:2:3:4:5 with ellipses lc rgb \"red\" title \"ellipses\"\n}";

    return result;
}

// Write log-message with date-time note.
void EMAlgorithm::writeLog(const string& message) {
    logs_a << timeLog() << "EMALGORITHM: " << message << endl;
}


///// FOREL /////

Forel::Forel(double RR, const vector<int>& pointss, Field* p_fieldd,
        ofstream& logs_al, int gl_stepp = 0, int framee = 0)
: points(pointss), logs_a(logs_al), R(RR), global_step(gl_stepp), frame(framee),
    f_clusters(logs_al, "FOREL, step #" + to_string(gl_stepp)), p_field(p_fieldd)
{
    writeLog("INIT");
}

Forel::~Forel() {
    points.clear();
    for (vector<double> cent : centroids) {
        cent.clear();
    }
    centroids.clear();
}

// Call algorithm.
vector <FindClusters> Forel::mainAlgorithm() {
    writeLog("Start mainAlgorithml");

    while (points.size() != 0) {
        f_clusters += oneNewCluster();
        writeLog("New cluster #" + to_string(step) + " created");
    }

    cout << "Find " << f_clusters.numClusters() << " clusters" << endl;
    
    return  vector<FindClusters> {f_clusters};
}

// Getter for point by index in forel-points.
Point& Forel::getPoint(int i) {
    return (*p_field)[points[i]];
}

// Remove point from unclustered by id.
void Forel::removePoint(int i) {
    for (int ind_in_unclust = 0; ind_in_unclust < points.size(); ind_in_unclust++) {
        if (i == points[ind_in_unclust]) {
            points.erase(points.begin() + ind_in_unclust);
            return;
        }
    }
}

// Find new Cluster and delete it from list.
Cluster Forel::oneNewCluster() {
    int new_center_id = randInt(points.size());
    vector<double> center = getPoint(new_center_id).getCoord();
    centroids.push_back(center);
    newFrame();
    Cluster neigh_cluster = findNeighbourhood(center);
    vector<double> new_center = neigh_cluster.findAverage();
    centroids.pop_back();
    centroids.push_back(new_center);
    newFrame();
    while (distPoints(center, new_center) > EPS) {
        center = new_center;
        neigh_cluster = findNeighbourhood(center);
        new_center = neigh_cluster.findAverage();
        centroids.pop_back();
        centroids.push_back(new_center);
        newFrame();
    }
    for (int ind = 0; ind < neigh_cluster.numPoints(); ind++) {
        removePoint(neigh_cluster[ind].id);
    }
    step++;
    return neigh_cluster;
}

// Find all points in the R-neighbourhood of the point
Cluster Forel::findNeighbourhood(const vector<double>& center) {
    Cluster res(step, logs_a, p_field);
    for (int other_point : points) {
        if (distPoints((*p_field)[other_point], center) < R) {
            res += other_point;
        }
    }
    return res;
}

void Forel::saveAnimation() {
    ifstream anim_templ("data/templates/forel_animate.template");
    ofstream anim("data/gnu_forel_animate.plt");

    string new_line;
    while (!anim_templ.eof()) {
        getline(anim_templ, new_line);
        anim << new_line << endl;
    }
    anim << "do for [i=0:" << to_string(frame - 1) << "] {" << endl << "\tplot ";
    for (int ind_k = 0; ind_k < step; ind_k++) {
        anim << "\t\t'data/forel/clusters_'.i.'.plt' index " << to_string(ind_k) <<
            " w p title \"#" << to_string(ind_k) << "\",\\" << endl;
    }
    anim << "\t\t'data/forel/circles_'.i.'.plt' using 1:2:3 with circles lc rgb \"red\" title \"circles\"\n}";
}

// Create new frame 'forel/clusters_i.plt' and 'forel/circles_i.plt'
void Forel::newFrame() {
    writeLog("New frame");
    ofstream file_clu("data/forel/clusters_" + to_string(frame) + ".plt");
    ofstream file_circ("data/forel/circles_" + to_string(frame) + ".plt");

    for (int unclust_p : points) {
        (*p_field)[unclust_p].print(file_clu);
    }

    file_clu << endl << endl;

    for (int nu_clu = 0; nu_clu < f_clusters.numClusters(); nu_clu++) {
        f_clusters[nu_clu].printGnu(file_clu);

        file_clu << endl << endl;
    }

    for (vector<double> cent : centroids) {
        file_circ << cent[0] << " " << cent[1] << " " << R << endl;
    }

    file_circ.close();
    file_clu.close();
    frame++;
}

// Write log-message with date-time note.
void Forel::writeLog(const string& message) {
    logs_a << timeLog() << "FOREL: " << message << endl;
}


///// FUNCTIONS /////

// Return determinant for 2d matrix.
double determinant(const vector<vector<double>>& sigma) {
    return sigma[0][0] * sigma[1][1] - sigma[0][1] * sigma[1][0];
}

// Return vector with eigenvectors and eigenvalues for 2d matrix
//  as {eivector1, eivector2, {eivalue1, eivalue2}}
//  empty vector if they don't exist.
vector<vector<double>> eigen(const vector<vector<double>>& sigma) {
    double a = sigma[0][0], c = sigma[1][0];
    vector<double> eigenvalues = findEigenvalues(sigma);
    vector<vector<double>> result;
    if (eigenvalues.size() == 0) { return result; }

    result.push_back(vector<double>{a - eigenvalues[0], c});
    result.push_back(vector<double>{a - eigenvalues[1], c});
    result.push_back(eigenvalues);
    return result;
}

// Return two or zero eigenvalues.
vector<double> findEigenvalues(const vector<vector<double>>& sigma) {
    double a = sigma[0][0], d = sigma[1][1];
    double descr = (a + d) * (a + d) - 4 * determinant(sigma);
    vector<double> lambdas;
    if (descr >= 0) {
        lambdas.push_back(((a + d) + sqrt(descr)) / 2.);
        lambdas.push_back(((a + d) - sqrt(descr)) / 2.);
    }
    return lambdas;
}