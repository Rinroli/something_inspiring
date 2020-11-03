// Copyright (c) 2020 Rinroli

#include "inter_contrl.h"
#include "func_file.h"
#include <stdio.h>
#include <unistd.h>
#include "time.h"
// #include <string>
#include <iostream>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

#define N 200
#define DELTA 0.1
#define K 10

///// CONTROLLER /////

Controller::Controller()
{
    logs.open("logs/logs_ctrl.txt", ios_base::app);
    logs << endl
        << "New session" << endl;
    writeLog("INIT");
}

// Write log-message with date-time note.
void Controller::writeLog(const string &message) {
    logs << timeLog() << message << endl;
}

Controller::~Controller()
{
    logs.close();
    writeLog("DELETE");
}

// Show help for command HELP.
bool Controller::showHelp()
{
    writeLog("Begin showHelp");
    ifstream help_file("help.txt");
    string readline;
    while (!help_file.eof()) {
        getline(help_file, readline);
        cout << readline << endl;
    }
    writeLog("\tEnd showHelp");
    return true;
}

// Enter readonly 'Analysis' mode.
bool Controller::enterAnalysis() {
    writeLog("Begin enterAnalysis");
    bool result = field.enterAnalysis();
    writeLog("\tEnd enterAnalysis, result=" + to_string(result));
    return result;
}

// Create incedent matrix for k.
bool Controller::createIncMatrix(double delta) {
    writeLog("Begin createIncMatrix");
    if (not field.ifReadonly()) {field.enterAnalysis();}
    field.binMatrix(delta);
    cout << "DONE!" << endl;
    writeLog("\tEnd createIncMatrix");
    return true;
}

// Create binary matrix for delta and k. Change mode if neccessary.
bool Controller::createDBMatrix(double delta, int k) {
    writeLog("Begin createIncDBMatrix");
    if (not field.ifReadonly()) {field.enterAnalysis();}
    field.binDBMatrix(delta, k);
    cout << "DONE!" << endl;
    writeLog("\tEnd createIncDBMatrix");
    return true;
}

// Show info about field and clouds for command INFO.
bool Controller::showInfoField() {
    writeLog("Begin showInfoField");
    if (not field.ifReadonly()) {field.enterAnalysis();}
    cout << "Info about field:\n"
         << "\tNumber of points: " << field.numPoints() << "\tNumber of clouds: " 
         << field.numClouds() << "\tNumber of FindClusters: " << field.numFClusters() << endl;
    cout << "Clouds:" << endl;
    for (int cl = 0; cl < field.numClouds(); ++cl) {
        field[cl].coutInfo();
    }
    cout << "Number of binary matrixes " << field.numBinMatrix() << endl;

    writeLog("\tEnd showInfoField");
    return true;
}

// Show info about FindClusters to the standart output.
bool Controller::showInfoFClusters() {
    writeLog("Begin showInfoFClusters");
    if (not field.ifReadonly()) {field.enterAnalysis();}
    cout << "Info about FindClusters:" << endl;
    for (int fc = 0; fc < field.numFClusters(); ++fc) {
        field.getFCluster(fc).coutInfo();
    }

    writeLog("\tEnd showInfoFClusters");
    return true;
}

// Show info about buffer.
bool Controller::showBuffer() {
    writeLog("Begin showBuffer");
    field.showBuffer();
    writeLog("\tEnd showBuffer");
    return true;
}

// Add cloud to the buffer (if not Analysis)
bool Controller::addToBuffer(int ind) {
    writeLog("Begin addBuffer");
    if (field.ifReadonly()) {
        cout << "Analysis mode. I can't." << endl;
        writeLog("\tEnd addBuffer (bad)");
        return false;
    }
    bool result = field.addToBuffer(ind);
    cout << "DONE!" << endl;
    writeLog("\tEnd addBuffer (success)");
    return result;
}

// Copy buffer to the field.
bool Controller::putBuffer() {
    writeLog("Begin putBuffer");
    if (field.ifReadonly()) {
        cout << "Analysis mode. I can't." << endl;
        writeLog("\tEnd putBuffer (bad)");
        return false;
    }
    bool result = field.putBuffer();
    cout << "DONE!" << endl;
    writeLog("\tEnd putBuffer (success)");
    return result;
}

// Rotate buffer on alpha angle.
bool Controller::rotateBuffer(double alpha) {
    writeLog("Begin rotateBuffer");
    if (field.ifReadonly()) {
        cout << "Analysis mode. I can't." << endl;
        writeLog("\tEnd rotateBuffer (bad)");
        return false;
    }
    field.rotateBuffer(alpha);
    cout << "DONE!" << endl;
    writeLog("\tEnd rotateBuffer (success)");
    return true;
}

// Move all points from the buffer.
bool Controller::moveBuffer(double x, double y) {
    writeLog("Begin moveBuffer");
    if (field.ifReadonly()) {
        cout << "Analysis mode. I can't." << endl;
        writeLog("\tEnd moveBuffer (bad)");
        return false;
    }
    field.moveBuffer(x, y);
    cout << "DONE!" << endl;
    writeLog("\tEnd moveBuffer (success)");
    return true;
}

// zoom all points from the buffer.
bool Controller::zoomBuffer(double k) {
    writeLog("Begin zoomBuffer");
    if (field.ifReadonly()) {
        cout << "Analysis mode. I can't." << endl;
        writeLog("\tEnd zoomBuffer (bad)");
        return false;
    }
    field.zoomBuffer(k);
    cout << "DONE!" << endl;
    writeLog("\tEnd zoomBuffer (success)");
    return true;
}

// Empty buffer.
bool Controller::emptyBuffer() {
    writeLog("Begin emptyBuffer");
    if (field.ifReadonly()) {
        cout << "Analysis mode. I can't." << endl;
        writeLog("\tEnd emptyBuffer (bad)");
        return false;
    }
    field.emptyBuffer();
    cout << "DONE!" << endl;
    writeLog("\tEnd emptyBuffer (success)");
    return true;
}

// Process input-data to gain cluster or cloud.
bool Controller::preHist(vector<string> args) {
    writeLog("Begin preHist");
    int first_arg = stod(args[1]);
    if (args[0] == "clu") {
        if (args.size() < 3) {throw -1;}
        if (first_arg >= field.numFClusters()) {
            writeLog("\t Wrong FCluster.");
            cout << "No FCluster, no histogram." << endl;
            return false;
        }
        FindClusters& cur_fc = (field.getFCluster(first_arg));
        if (stod(args[2]) >= cur_fc.numClusters()) {
            writeLog("\t Wrong cluster.");
            cout << "No cluster, no histogram." << endl;
            return false;
        }
        return saveHist(cur_fc[stod(args[2])]);
    } else {
        if (first_arg >= field.numClouds()) {
            writeLog("\t Wrong cloud.");
            cout << "No cloud, no histogram." << endl;
            return false;
        }
        return saveHist(field[first_arg]);
    }
}

// Generate and save histogram of points from cluster.
bool Controller::saveHist(Cluster cluster) {
    writeLog("Begin saveHist (cluster)");
    vector<int> fr_x(N), fr_y(N);
    double max_x, min_x, max_y, min_y;
    double step_x, step_y;
    if (field.numPoints() == 0) {
        cout << "No points." << endl;
        writeLog("\t No points.");
        return false;
    }
    else if (cluster.numPoints() == 1) {
        cout << "Only one point." << endl;
        writeLog("\t Only one point.");
        return false;
    }

    vector<double> some_point = cluster[0].getCoord();

    max_x = some_point[0];
    max_y = some_point[1];
    min_x = some_point[0];
    min_y = some_point[1];

    for (int i = 0; i < cluster.numPoints(); ++i) {
        some_point = cluster[i].getCoord();
        if (some_point[0] > max_x) {max_x = some_point[0];}
        if (some_point[0] < min_x) {min_x = some_point[0];}
        if (some_point[1] > max_y) {max_y = some_point[1];}
        if (some_point[1] < min_y) { min_y = some_point[1]; }
    }

    step_x = (max_x - min_x) / N;
    step_y = (max_y - min_y) / N;

    for (int i = 0; i < cluster.numPoints(); ++i) {
        some_point = cluster[i].getCoord();
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
    cout << "DONE!" << endl;
    writeLog("\tEnd saveHist");
    return true;
}

// Save histogram to the file for command HIST.
bool Controller::saveHist()
{
    writeLog("Begin saveHist (field)");
    Cluster field_cluster(-1, field.logs, &field);
    for (int i = 0; i < field.numPoints(); i++) {
        field_cluster += i;
    }
    writeLog("\tEnd");

    return saveHist(field_cluster);
}

// Create histogram on edges of the tree.
bool Controller::streeHist() {
    writeLog("Begin streeHist");
    if (field.p_tree == NULL) {
        cout << "No tree yet" << endl;
        writeLog("\tEnd streeHist(no tree)");
        return false;
    }
    vector<double> all_dist = field.p_tree->allDist();
    double max_d = 0;
    for (double dist : all_dist) {
        if (dist > max_d) {max_d = dist;}
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
    hist.open("data/tree_hist.plt");
    for (int k = 0; k < N; ++k) {
        hist << k * step << "\t" << freq[k] << endl;
    }
    writeLog("\tEnd streeHist");
    cout << "DONE!" << endl;
    return true;
}

// Find good delta for wave algorithm.
bool Controller::findR() {
    writeLog("Begin findR");
    if (field.p_tree == NULL) {
        cout << "No tree yet" << endl;
        writeLog("\tEnd findR(no tree)");
        return false;
    }
    vector<double> all_dist = field.p_tree->allDist();

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
    cout << "Result delta = " << delta / 2 << endl;
    cout << "DONE!" << endl;
    return true;
}

// Save field to the file for command SAVE.
bool Controller::printField(bool clouds = true, int i = 0)
{
    string what;
    if (clouds) {
        what = "Clouds";
    } else {
        FindClusters cur_clu = field.getFCluster(i);
        what = "Clusters, " + cur_clu.source;
    }
    writeLog("Begin printField " + what);
    string file_name = (clouds) ? "output.plt" : "clusters.plt";
    string main_file_name = (clouds) ? "gnuplot.plt" : "clus_gnu.plt";
    int nu_groups = (clouds) ? field.numClouds() : field.getFCluster(i).numClusters();

    ofstream out_f("data/" + file_name);
    if (clouds) {
        field.print(out_f);
    } else {
        field.print(i, out_f);
    }
    out_f.close();

    ofstream gnu("data/" + main_file_name);
    gnu << "set title \"" << what << "\"\nplot ";
    for (int i = 0; i < nu_groups; ++i) {
        gnu << "'data/" << file_name << "' index " << i << " w p title \"" << i << "\"";
        if (i != nu_groups - 1) {
            gnu << ",\\";
        }
        gnu << endl;
    }
    gnu << "pause -1\n"
        << endl;
    gnu.close();
    writeLog("\tEnd printField");
    cout << "DONE!" << endl;
    return true;
}

// Generate cloud for the command GEN_CLOUD.
bool Controller::genCloud(double mX, double mY, double sX, double sY)
{
    writeLog("Begin genCloud");
    if (field.createCloud(mX, mY, sX, sY)) {
        cout << "DONE!" << endl;
        writeLog("\tEnd genCloud (success)");
        return true;
    } else {
        cout << "Access denied" << endl;
        writeLog("\tEnd genCloud (forbidden)");
        return false;
    }
    return true;
}

// Create new cluster selection by wave algorithm. 
bool Controller::waveClusters(int i)
{
    writeLog("Begin waveClusters");
    if (field.numBinMatrix() <= i) { return false; }
    if (not field.ifReadonly()) {field.enterAnalysis();}
    WaveClusters wave(field.getBinMatrix(i), field.logs_a, &field);
    field.addFC(wave.mainAlgorithm());

    cout << "Saved!" << endl;
    return true;
}

// Create clusters by k-Means algorithm.
bool Controller::kMeans(int n) {
    writeLog("Begin kMeans");
    if (not field.ifReadonly()) {field.enterAnalysis();}
    KMeans new_k_means(n, field, field.logs_a);
    field.addFC(new_k_means.mainAlgorithm());
    writeLog("\tEnd kMeans");
    cout << "DONE!" << endl;
    return true;
}

// Create clusteres by EM-algorithm.
bool Controller::eMAlgorithm(int n) {
    writeLog("Begin eMalgorithm");
    if (not field.ifReadonly()) {field.enterAnalysis();}
    EMAlgorithm new_em(n, field, field.logs_a);
    field.addFC(new_em.mainAlgorithm());
    writeLog("\tEnd eMalgorithm");
    cout << "DONE!" << endl;
    return true;
}

// Print to the file all points and edges by binary matrix.
bool Controller::displayGraph(int i) {
    writeLog("Begin displayPoints");
    if (field.numBinMatrix() <= i) { return false; }
    field.drawBinGraph(i);
    cout << "DONE! Saved!" << endl;
    return true;
}

// Create minimal spanning tree for field.
bool Controller::minSpanTree() {
    writeLog("Begin minSpanTree");
    if (not field.ifReadonly()) { field.enterAnalysis(); }
    field.minSpanTree();
    cout << "DONE!" << endl;
    return true;
}

///// INTERFACE /////

Interface::Interface(int idd)
 {
    id = idd;

    logs.open("logs/logs_interface.txt", ios_base::app);
    logs << endl
        << "New session" << endl;
    writeLog("INIT");
}

Interface::~Interface()
{
    logs.close();
    writeLog("DELETE");
}

// Get command from the user and compile it.
bool Interface::runCommand(string command)
{
    writeLog("GET COMMAND <" + command + ">");
    cout << command << ": ";
    bool result = false;
    try
    {
        char* s = new char[command.size() + 1];
        strcpy(s, command.c_str());
        char* pch = strtok(s, ", ()");
        string com(pch);
        vector<string> args;
        while (pch != NULL) {
            pch = strtok(NULL, " ,()");
            if (pch == NULL) {
                break;
            }
            args.push_back(pch);
        }
        if (command == "HELP") {
            result = ctrl.showHelp();
        }

        else if ((com == "GEN_CLOUD") | (com == "GC")) {
            if (args.size() == 0) {result = ctrl.genCloud(0, 0, 1, 1);}
            else if (args.size() < 4) {throw - 1;} 
            else {
                result = ctrl.genCloud(stod(args[0]), stod(args[1]),
                    stod(args[2]), stod(args[3]));
            }
        }

        else if (com == "BINARY") {
            if (args.size() == 0) {result = ctrl.createIncMatrix(DELTA);}
            else {result = ctrl.createIncMatrix(stod(args[0]));}
        }

        else if (com == "DBSCAN") {
            if (args.size() == 0) {result = ctrl.createDBMatrix(DELTA, K);}
            else if (args.size() < 2) {throw - 1;} 
            else {result = ctrl.createDBMatrix(stod(args[0]), stod(args[1]));}
        }

        else if ((com == "WAVE") | (com == "DBWAVE")) {
            if (args.size() == 0) {result = ctrl.waveClusters(-1);}
            else {result = ctrl.waveClusters(stod(args[0]));}
        }

        else if ((com == "DIBINARY") | (com == "DIDBSCAN") | (com == "DIB") | (com == "DID")) {
            if (args.size() == 0) {result =  ctrl.displayGraph(-1);}
            else { result = ctrl.displayGraph(stod(args[0])); }
        }

        else if (com == "HIST") {
            if (args.size() == 0) {
                result = ctrl.saveHist();
            } else if (args.size() == 1) {throw - 1;}
            else {result = ctrl.preHist(args);}
        }

        else if (com == "KMEANS") {
            if (args.size() == 0) { result = ctrl.kMeans(25); }
            else { result = ctrl.kMeans(stod(args[0])); }
        }

        else if (com == "EM") {
            if (args.size() == 0) { result = ctrl.eMAlgorithm(25); }
            else { result = ctrl.eMAlgorithm(stod(args[0])); }
        }

        else if (com == "SAVE") {
            if (args.size() == 0) { result = ctrl.printField(); }
            else { result = ctrl.printField(false, stod(args[0])); }
        }

        else if (com == "ADDB") {
            if (args.size() == 0) { result = ctrl.addToBuffer(-1); }
            else { result = ctrl.addToBuffer(stod(args[0])); }
        }

        else if (com == "ROTB") {
            if (args.size() < 1) { throw -1; }
            result = ctrl.rotateBuffer(stod(args[0]));
        }

        else if (com == "MOVEB") {
            if (args.size() < 2) { throw - 1; }
            result = ctrl.moveBuffer(stod(args[0]), stod(args[1]));
        }

        else if (com == "ZOOMB") {
            if (args.size() < 1) { throw - 1; }
            result = ctrl.zoomBuffer(stod(args[0]));
        }

        else if (com == "EXIT") {
            cout << "Okay..." << endl;
            writeLog("\tCorrect <" + command + ">");
            return false;
        } else if (com == "INFO") {
            result = ctrl.showInfoField();
        } else if (com == "INFOFC") {
            result = ctrl.showInfoFClusters();
        } else if ((com == "MATRIX") | (com == "ANALYSIS")) {
            result = ctrl.enterAnalysis();
        } else if (com == "STREE") {
            result = ctrl.minSpanTree();
        } else if (com == "STRHIST") {
            result = ctrl.streeHist();
        } else if (com == "FINDR") {
            result = ctrl.findR();
        } else if (com == "SHOWB") {
            result = ctrl.showBuffer();
        } else if (com == "PUTB") {
            result = ctrl.putBuffer();
        } else if (com == "EMPTYB") {
            result = ctrl.emptyBuffer();
        }

        else {
            cout << "No such command, sorry." << endl
                << "Would you like to get some HELP?" << endl;
        }
    }
    catch (int i) {
        result = false;
        if (i == -1) {
            cout << "Wrong command."
                 << "nWould you like to get some HELP?" << endl;
        } else if (i == -2) {
            cout << "Exit." << endl;
            writeLog("\tIncorrect <" + command + ">");
            return false;
        }
        else {
            throw;
        }
    }
    catch (...) {
        result = false;
        cout << "Somethig wrong, sorry."
             << "Would you like to get some HELP?" << endl;
    }
    if (result) { writeLog("\tCorrect <" + command + ">"); } 
    else { writeLog("\tIncorrect <" + command + ">"); }
    return true;
}

// Write log-message with date-time note.
void Interface::writeLog(const string& message) {
    logs << timeLog() << message << endl;
}