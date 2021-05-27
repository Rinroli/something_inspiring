// Copyright (c) 2020 Rinroli

#include "Controller.h"

using namespace std;

#define N 1000

Controller::Controller(vector<bool> if_logs, vector<string> name_logs) {
    field = new Field(if_logs, name_logs, message);
    if (if_logs[1]) {
        logs.open("logs/" + name_logs[1], ios_base::app);
        logs << endl << "New session" << endl;
        writeLog("INIT");
    }
    message << "M:\n";
    // message_buffer[0] = 0;
}

// Write log-message with date-time note.
void Controller::writeLog(const string& message) {
    logs << timeLog() << message << endl;
}

Controller::~Controller()
{
    writeLog("DELETE");
    logs.close();
    delete field;
}

// Begin test, change main work directories.
void Controller::beginTest(const string& output_dir, const string& output_na,
    const string& gen_f) {
    writeLog("beginTest");
    if_test = true;
    output_directory = "tests/" + output_dir;
    output_name = output_na;
    if (gen_f == "none") {
        gen_file = output_directory;
    } else {
        gen_file = gen_f;
    }
    field->beginTest(output_directory, output_name);
    writeLog("\tendTest");
}

// Enter readonly 'Analysis' mode.
bool Controller::enterAnalysis() {
    writeLog("Begin enterAnalysis");
    bool result = field->enterAnalysis();
    writeLog("\tEnd enterAnalysis, result=" + to_string(result));
    return result;
}

// Create incedent matrix for k.
bool Controller::createIncMatrix(double delta) {
    writeLog("Begin createIncMatrix");
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    field->binMatrix(delta);
    message << "DONE!" << endl;
    writeLog("\tEnd createIncMatrix");
    return true;
}

// Create binary matrix for delta and k. Change mode if neccessary.
bool Controller::createDBMatrix(double delta, int k) {
    writeLog("Begin createIncDBMatrix");
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    field->binDBMatrix(delta, k);
    message << "DONE!" << endl;
    writeLog("\tEnd createIncDBMatrix");
    return true;
}

// Show info about field->and clouds for command INFO.
bool Controller::showInfoField() {
    writeLog("Begin showInfoField");
    if (not field->ifReadonly()) { field->enterAnalysis(); }

    // stringstream str_to_cli;
    message << "Info about field->\n"
        << "\tNumber of points: " << field->numPoints() << "\tNumber of clouds: "
        << field->numClouds() << "\tNumber of FindClusters: " << field->numFClusters() << endl;
    message << "Clouds:" << endl;
    for (int cl = 0; cl < field->numClouds(); ++cl) {
        field->getCloud(cl).coutInfo(message);
    }
    message << "Number of binary matrixes " << field->numBinMatrix() << endl;

    return true;
}

// Show info about FindClusters to the standart output.
bool Controller::showInfoFClusters() {
    writeLog("Begin showInfoFClusters");
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    message << "Info about FindClusters:" << endl;
    for (int fc = 0; fc < field->numFClusters(); ++fc) {
        field->getFCluster(fc).coutInfo(message);
    }

    writeLog("\tEnd showInfoFClusters");
    return true;
}

// Show info about buffer.
bool Controller::showBuffer() {
    writeLog("Begin showBuffer");
    field->showBuffer();
    writeLog("\tEnd showBuffer");
    return true;
}

// Add cloud to the buffer (if not Analysis)
bool Controller::addToBuffer(int ind) {
    writeLog("Begin addBuffer");
    if (field->ifReadonly()) {
        message << "Analysis mode. I can't." << endl;
        writeLog("\tEnd addBuffer (bad)");
        return false;
    }
    bool result = field->addToBuffer(ind);
    message << "DONE!" << endl;
    writeLog("\tEnd addBuffer (success)");
    return result;
}

// Copy buffer to the field->
bool Controller::putBuffer() {
    writeLog("Begin putBuffer");
    if (field->ifReadonly()) {
        message << "Analysis mode. I can't." << endl;
        writeLog("\tEnd putBuffer (bad)");
        return false;
    }
    bool result = field->putBuffer();
    message << "DONE!" << endl;
    writeLog("\tEnd putBuffer (success)");
    return result;
}

// Rotate buffer on alpha angle.
bool Controller::rotateBuffer(double alpha) {
    writeLog("Begin rotateBuffer");
    if (field->ifReadonly()) {
        message << "Analysis mode. I can't." << endl;
        writeLog("\tEnd rotateBuffer (bad)");
        return false;
    }
    field->rotateBuffer(alpha);
    message << "DONE!" << endl;
    writeLog("\tEnd rotateBuffer (success)");
    return true;
}

// Move all points from the buffer.
bool Controller::moveBuffer(double x, double y) {
    writeLog("Begin moveBuffer");
    if (field->ifReadonly()) {
        message << "Analysis mode. I can't." << endl;
        writeLog("\tEnd moveBuffer (bad)");
        return false;
    }
    field->moveBuffer(x, y);
    message << "DONE!" << endl;
    writeLog("\tEnd moveBuffer (success)");
    return true;
}

// zoom all points from the buffer.
bool Controller::zoomBuffer(double k) {
    writeLog("Begin zoomBuffer");
    if (field->ifReadonly()) {
        message << "Analysis mode. I can't." << endl;
        writeLog("\tEnd zoomBuffer (bad)");
        return false;
    }
    field->zoomBuffer(k);
    message << "DONE!" << endl;
    writeLog("\tEnd zoomBuffer (success)");
    return true;
}

// Empty buffer.
bool Controller::emptyBuffer() {
    writeLog("Begin emptyBuffer");
    if (field->ifReadonly()) {
        message << "Analysis mode. I can't." << endl;
        writeLog("\tEnd emptyBuffer (bad)");
        return false;
    }
    field->emptyBuffer();
    message << "DONE!" << endl;
    writeLog("\tEnd emptyBuffer (success)");
    return true;
}

// Process input-data to gain cluster or cloud.
bool Controller::preHist(vector<string> args) {
    writeLog("Begin preHist");
    int first_arg = stod(args[1]);
    if (args[0] == "clu") {
        if (args.size() < 3) { throw - 1; }
        if (first_arg >= field->numFClusters()) {
            writeLog("\t Wrong FCluster.");
            message << "No FCluster, no histogram." << endl;
            return false;
        }
        FindClusters& cur_fc = (field->getFCluster(first_arg));
        if (stod(args[2]) >= cur_fc.numClusters()) {
            writeLog("\t Wrong cluster.");
            message << "No cluster, no histogram." << endl;
            return false;
        }
        return saveHist(cur_fc[stod(args[2])]);
    }
    else {
        if (first_arg >= field->numClouds()) {
            writeLog("\t Wrong cloud.");
            message << "No cloud, no histogram." << endl;
            return false;
        }
        return saveHist(field->getCloud(first_arg));
    }
}

// Generate and save histogram of points from cluster.
bool Controller::saveHist(Cluster cluster) {
    writeLog("Begin saveHist (cluster)");
    vector<int> fr_x(N), fr_y(N);
    double max_x, min_x, max_y, min_y;
    double step_x, step_y;
    if (field->numPoints() == 0) {
        message << "No points." << endl;
        writeLog("\t No points.");
        return false;
    }
    else if (cluster.numPoints() == 1) {
        message << "Only one point." << endl;
        writeLog("\t Only one point.");
        return false;
    }

    vector<double> some_point = cluster.getOnePointCoords();

    max_x = some_point[0];
    max_y = some_point[1];
    min_x = some_point[0];
    min_y = some_point[1];

    for (int i : cluster.getPoints()) {
        some_point = field->getPoint(i).getCoord();
        if (some_point[0] > max_x) { max_x = some_point[0]; }
        if (some_point[0] < min_x) { min_x = some_point[0]; }
        if (some_point[1] > max_y) { max_y = some_point[1]; }
        if (some_point[1] < min_y) { min_y = some_point[1]; }
    }

    step_x = (max_x - min_x) / N;
    step_y = (max_y - min_y) / N;

    for (int i : cluster.getPoints()) {
        some_point = field->getPoint(i).getCoord();
        fr_x[int((some_point[0] - min_x) / (step_x))]++;
        fr_y[int((some_point[1] - min_y) / (step_y))]++;
    }

    ofstream hist;
    hist.open("data/hist.plt");
    for (int k = 0; k < N; ++k) {
        hist << k * step_x + min_x << "\t" << fr_x[k] << endl;
    }
    hist << "\n\n"
        << endl;
    for (int n = 0; n < N; ++n) {
        hist << n * step_y + min_y << "\t" << fr_y[n] << endl;
    }
    hist.close();

    message << "DONE!" << endl;
    writeLog("\tEnd saveHist");
    return true;
}

// Save histogram to the file for command HIST.
bool Controller::saveHist()
{
    writeLog("Begin saveHist (field)");
    Cluster field_cluster(-1, field->logs, field);
    for (int i = 0; i < field->numPoints(); i++) {
        field_cluster += i;
    }
    writeLog("\tEnd");

    return saveHist(field_cluster);
}

// Create histogram on edges of the tree.
bool Controller::streeHist() {
    writeLog("Begin streeHist");
    if (field->p_tree == NULL) {
        message << "No tree yet" << endl;
        writeLog("\tEnd streeHist(no tree)");
        return false;
    }
    vector<double> all_dist = field->p_tree->allDist();
    double max_d = 0;
    for (double dist : all_dist) {
        if (dist > max_d) { max_d = dist; }
    }
    double step = max_d / N;

    vector<double> freq(N);
    for (double dist : all_dist) {
        if (dist == max_d) {
            freq[N - 1]++;
            continue;
        }
        freq[int(dist / step)]++;
    }

    ofstream hist;
    // hist.open("data/tree_hist.plt");
    hist.open(output_directory + "/" + output_name + "_hist.plt");
    for (int k = 0; k < N; ++k) {
        hist << k * step << "\t" << freq[k] << endl;
    }
    ofstream gnu(output_directory + "/gnu_tree_hist.plt");
    if (if_test) {
        gnu << "set terminal png size 900, 800 enhanced font \"Helvetica,20\"" << endl
            << "set output \"" << gen_file << "/" << output_name << "_hist.png\"" << endl
            << "set title \"HISTOGRAM\"" << endl << "set style fill transparent solid 0.5" << endl
            << "plot 'tests/Algorithm/spanning_tree/spanning_tree_hist.plt'"
            << " index 0 title \"distances\" w boxes" << endl;
    }
    writeLog("\tEnd streeHist");
    message << "DONE!" << endl;
    return true;
}

// Find good delta for wave algorithm.
bool Controller::findR() {
    writeLog("Begin findR");
    if (field->p_tree == NULL) {
        message << "No tree yet" << endl;
        writeLog("\tEnd findR(no tree)");
        return false;
    }
    vector<double> all_dist = field->p_tree->allDist();

    double aver_dist = 0, max_dist;
    for (double edge : all_dist) {
        aver_dist += edge;
        if (max_dist < edge) { max_dist = edge; }
    }
    aver_dist /= all_dist.size();

    double delta = max_dist;
    for (double edge : all_dist) {
        if ((edge > 5 * aver_dist) & (edge < delta)) {
            delta = edge;
        }
    }

    writeLog("\tEnd findR");
    message << "Result delta = " << delta / 2 << endl;
    message << "DONE!" << endl;
    return true;
}

// Save field->to the file for command SAVE.
bool Controller::printField(bool clouds, int i)
{
    string what;
    if (clouds) {
        what = "Clouds";
    }
    else {
        FindClusters cur_clu = field->getFCluster(i);
        what = "Clusters, " + cur_clu.source;
    }
    writeLog("Begin printField " + what);
    string gnu_file_name, data_output_name;
    if (not if_test) {
        data_output_name = (clouds) ? "output.plt" : "clusters.plt";
        gnu_file_name = (clouds) ? "gnuplot.plt" : "clus_gnu.plt";
    }
    else {
        gnu_file_name = "gnu_" + output_name + ".plt";
        data_output_name = "data_" + output_name + ".plt";
    }
    int nu_groups = (clouds) ? field->numClouds() : field->getFCluster(i).numClusters();

    writeLog("Files: " + data_output_name + " " + gnu_file_name);
    writeLog("Directory: " + output_directory);

    ofstream out_f(output_directory + "/" + data_output_name);
    if (clouds) {
        field->print(out_f);
    }
    else {
        field->print(i, out_f);
    }
    out_f.close();

    ofstream gnu(output_directory + "/" + gnu_file_name);
    if (if_test) {
        gnu << "set terminal png size 900,800 enhanced font \"Helvetica,20\"" << endl;
        gnu << "set nokey" << endl;
        gnu << "set output \"" << gen_file << "/"
            << output_name << ".png\"" << endl;
    }
    gnu << "set title \"" << what << "\"\nset size ratio -1\nplot ";
    for (int i = 0; i < nu_groups; ++i) {
        gnu << "'" << output_directory << "/" << data_output_name << "' index " <<
            i << " w p title \"" << i << "\"";
        if (i != nu_groups - 1) {
            gnu << ",\\";
        }
        gnu << endl;
    }
    if (not if_test) {
        gnu << "pause -1\n" << endl;
    }
    gnu.close();
    writeLog("\tEnd printField");
    message << "DONE!" << endl;
    return true;
}

// Generate cloud for the command GEN_CLOUD.
bool Controller::genCloud(double mX, double mY, double sX, double sY, int nu_points)
{
    writeLog("Begin genCloud");
    if (field->createCloud(mX, mY, sX, sY, nu_points)) {
        message << "DONE!" << endl;
        writeLog("\tEnd genCloud (success)");
        return true;
    }
    else {
        message << "Access denied" << endl;
        writeLog("\tEnd genCloud (forbidden)");
        return false;
    }
    return true;
}

// Create new cluster selection by wave algorithm. 
bool Controller::waveClusters(int i)
{
    writeLog("Begin waveClusters");
    if (field->numBinMatrix() <= i) { return false; }
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    WaveClusters wave(field->getBinMatrix(i), field->logs_a, field);
    field->addFC(wave.mainAlgorithm());

    message << "Saved!" << endl;
    return true;
}

// Create clusters by k-Means algorithm.
bool Controller::kMeans(int n) {
    writeLog("Begin kMeans");
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    KMeans new_k_means(n, *field, field->logs_a);
    field->addFC(new_k_means.mainAlgorithm());
    writeLog("\tEnd kMeans");
    message << "DONE!" << endl;
    return true;
}

// Create clusters by k-Means with kernels algorithm.
bool Controller::kerKMeans(int k, int p) {
    writeLog("Begin kerKMeans");
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    KerKMeans new_ker_kmeans(k, p, *field, field->logs_a);
    field->addFC(new_ker_kmeans.mainAlgorithm());
    writeLog("\tEnd kerKMeans");
    message << "DONE!" << endl;
    return true;
}

// Create clusteres by EM-algorithm.
bool Controller::eMAlgorithm(int n) {
    writeLog("Begin eMalgorithm");
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    EMAlgorithm new_em(n, *field, field->logs_a);
    field->addFC(new_em.mainAlgorithm());
    writeLog("\tEnd eMalgorithm");
    message << "DONE!" << endl;
    return true;
}

// Create clusteres by FOREL-algorithm.
bool Controller::forelAlg(double R) {
    writeLog("Begin FOREL");
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    Forel new_forel(R, field->points, field, field->logs_a, 0, 0, 0);
    vector<FindClusters> result = new_forel.mainAlgorithm();
    for (FindClusters one_fc : result) {
        for (int clu_ind = 0; clu_ind < one_fc.numClusters(); clu_ind++) {
            one_fc.clusters[clu_ind].p_field_points = &(field->points);
        }
        field->addFC(one_fc);
    }
    writeLog("\tEnd FOREL");
    message << "DONE!" << endl;
    return true;
}

// Create clusteres by Hierarch-algorithm.
bool Controller::hierarchClustering(int k) {
    writeLog("Begin HIERARCH");
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    Hierarch new_hierarch(k, field, field->logs_a);
    field->addFC(new_hierarch.mainAlgorithm());
    writeLog("\tEnd HIERARCH");
    message << "DONE!" << endl;
    return true;
}

// Print to the file all points and edges by binary matrix.
bool Controller::displayGraph(int i) {
    writeLog("Begin displayGraph");
    if (field->numBinMatrix() <= i) { return false; }
    field->drawBinGraph(i);
    ifstream graph_template("data/templates/" + output_name + "_graph.template");
    ofstream gnu(output_directory + "/gnu_graph.plt");
    if (if_test) {
        gnu << "set terminal png size 900, 800 enhanced font \"Helvetica,20\"" << endl
            << "set output \"" << gen_file << "/" << output_name << "_graph.png\"" << endl;
    }
    gnu << "set title \"" << output_name << " graph" << "\"\nset size ratio -1\n";
    string new_line;
    while (!graph_template.eof()) {
        getline(graph_template, new_line);
        gnu << new_line << endl;
    }
    if (not if_test) {
        gnu << "pause -1\n" << endl;
    }
    gnu.close();
    writeLog("\tEnd dispayGraph");
    message << "DONE! Saved!" << endl;
    return true;
}

// Create minimal spanning tree for field->
bool Controller::minSpanTree() {
    writeLog("Begin minSpanTree");
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    field->minSpanTree();
    ofstream gnu(output_directory + "/gnu_tree.plt");
    if (if_test) {
        gnu << "set terminal png size 900,800 enhanced font \"Helvetica,20\"" << endl
            << "set output \"" << gen_file << "/" << output_name << ".png\"" << endl
            << "set size ratio -1" << endl
            << "plot 'tests/Algorithm/spanning_tree/spanning_tree_data.plt' with lines lc rgb \"black\" notitle" << endl;
    }
    message << "DONE!" << endl;
    return true;
}

// Return current message for client.
string Controller::curMessage() {
    writeLog("Show saved message");
    // message_buffer[message_len] = 0;
    // return message_buffer;
    return message.str();
}

// Clean current message.
void Controller::cleanMessage() {
    message.str("M:\n");
    writeLog("Message cleaned");
    // message << "M:\n";
}

// Create Delaunay triangulation for field
bool Controller::delaunayTriangulation() {
    writeLog("Begin delaunayTriangulation");
    if (not field->ifReadonly()) { field->enterAnalysis(); }
    Delaunay delaunay(field, field->logs_a);
    field->p_triangulation = delaunay.mainAlgorithm();
    writeLog("\tEnd delaunayTriangulation - find " +
        to_string(field->p_triangulation->size()) + " triangles");
    message << "DONE!" << endl;
    return true;
}


// Make point value prediction
bool Controller::pointPrediction(double x, double y) {
    writeLog("Begin pointPrediction");
    if (not field->p_triangulation) {
        writeLog("There is no triangulations!");
        message << "There is no triangulations!" << endl;
        return false;
    }
    Prediction predict(vector<double>{x, y}, field->p_triangulation, field, field->logs_a);
    
    if (not predict.real) {
        writeLog("Out of triangulation");
        message << "Out of triangulation" << endl;
        return false;
    }
    
    double result = predict.predictPoint();
    writeLog("Find value <" + to_string(result) + ">");
    message << endl << "\tFind value <" +
        colorString(to_string(result)) + ">" << endl;
    message << "\tReal value <" << 
        colorString(to_string(funcValue(vector<double>{x, y}))) << ">" << endl;
    return true;
}

// Find efficiency factor as \sum(val_real - val_predicted)^2 / \sum (val_real - val_mean)^2
bool Controller::predictionCoeff() {
    writeLog("Begin predictCoeff >> " + to_string(field->p_triangulation->size()));
    if (not field->p_triangulation) {
        writeLog("There is no triangulations!");
        message << "There is no triangulations!" << endl;
        return false;
    }

    int nu_real_p = 0;
    double sum_square_err = 0;
    double sum_square_dev = 0;
    double sum_val = 0;
    vector<double> real_val;
    
    for (int ind_poi = 0; ind_poi < field->numPoints(); ind_poi++) {
        Delaunay* cur_del = new Delaunay(field, field->logs_a);
        // Triangulation* cur_tr = field->p_triangulation->withoutPoint(ind_poi);
        
        Prediction predict(field->getPoint(ind_poi).getCoord(),
            cur_del->mainAlgorithm(ind_poi), field, field->logs_a);

        if (not predict.real) {
            writeLog("Out of triangulation");
        } else {
            double result = predict.predictPoint();
            cout <<"P:" << result << " <> R:" << funcValue(field->getPoint(ind_poi).getCoord()) << endl;
            real_val.push_back(result);
            sum_val += result;
            sum_square_err += pow(funcValue(field->getPoint(ind_poi).getCoord()) - result, 2);
        }
        delete cur_del;
    }

    double mean_val = sum_val / real_val.size();

    for (double r_val : real_val) {
        sum_square_dev += pow(r_val - mean_val, 2);
    }

    message << endl << "\tCoefficient <" +
        colorString(to_string(1 - sum_square_err / sum_square_dev)) + ">" << endl;

    writeLog("\t End predict Coeff <" + to_string(1 - sum_square_err / sum_square_dev) + ">");
    return true;
}