// Copyright (c) 2020 Rinroli

#include "inter_contrl.h"
#include "func_file.h"
#include "algorith.h"
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

#define N 100
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

void Controller::writeLog(const string &message)
{
    /* Write log-message with date-time note. */
    logs << timeLog() << message << endl;
}

Controller::~Controller()
{
    logs.close();
    writeLog("DELETE");
}

bool Controller::showHelp()
{
    /* Show help for command HELP. */
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

bool Controller::enterAnalysis()
{
    /* Enter readonly 'Analysis' mode. */
    writeLog("Begin enterAnalysis");
    bool result = field.enterAnalysis();
    writeLog("\t End enterAnalysis, result=" + to_string(result));
    return result;
}

bool Controller::createIncMatrix(double delta)
{
    /* Create incedent matrix for k. */
    writeLog("Begin createIncMatrix");
    if (not field.ifReadonly()) {field.enterAnalysis();}
    field.binMatrix(delta);
    cout << "Done!" << endl;
    writeLog("End createIncMatrix");
    return true;
}

bool Controller::createDBMatrix(double delta, int k)
{
    /* Create binary matrix for delta and k. Change mode if neccessary. */
    writeLog("Begin createIncDBMatrix");
    if (not field.ifReadonly()) {field.enterAnalysis();}
    field.binDBMatrix(delta, k);
    cout << "Done!" << endl;
    writeLog("End createIncDBMatrix");
    return true;
}

bool Controller::showInfoField()
{
    /* Show info about field and clouds for command INFO. */
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

bool Controller::showInfoFClusters()
{
    /* Show info about FindClusters to the standart output. */
    writeLog("Begin showInfoFClusters");
    if (not field.ifReadonly()) {field.enterAnalysis();}
    cout << "Info about FindClusters:" << endl;
    for (int fc = 0; fc < field.numFClusters(); ++fc) {
        field.getFCluster(fc).coutInfo();
    }

    writeLog("\tEnd showInfoFClusters");
    return true;
}

bool Controller::preHist(vector<string> args)
{
    /* Process input-data to gain cluster or cloud. */
    if (args[0] == "clu") {
        if (stod(args[1]) >= field.numFClusters()) {
            writeLog("\t Wrong cluster.");
            cout << "Wrong id of cluster, no histogram." << endl;
            return false;
        }
        return true;
    } else {
        if (stod(args[1]) >= field.numClouds()) {
            writeLog("\t Wrong cloud.");
            cout << "Wrong id of cloud, no histogram." << endl;
            return false;
        }
        return saveHist(field[stod(args[1])]);
    }
}

bool Controller::saveHist(Cluster cluster)
{
    /* Generate and save histogram of points from cluster. */
    writeLog("Begin saveHist (cluster)");
    vector<int> fr_x(N), fr_y(N);
    double max_x, min_x, max_y, min_y;
    double step_x, step_y;
    if (field.numPoints() == 0) {
        writeLog("\t No points.");
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
    cout << "Done!" << endl;
    writeLog("\tEnd saveHist");
    return true;
}

bool Controller::saveHist()
{
    /* Save histogram to the file for command HIST. */
    writeLog("Begin saveHist (field)");
    Cluster field_cluster(-1, field.logs, &field);
    for (int i = 0; i < field.numPoints(); i++) {
        field_cluster += i;
    }

    return saveHist(field_cluster);
}

bool Controller::printField(bool clouds = true, int i = 0)
{
    /* Save field to the file for command SAVE. */
    string what = (clouds) ? "(clouds)" : "(clusters)";
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

bool Controller::genCloud(double mX, double mY, double sX, double sY)
{
    /* Generate cloud for the command GEN_CLOUD. */
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

bool Controller::waveClusters(int i)
{
    /* Create new cluster selection by wave algorithm. */
    writeLog("Begin waveClusters");
    if (field.numBinMatrix() <= i) { return false; }
    if (not field.ifReadonly()) {field.enterAnalysis();}
    WaveClusters wave(field.getBinMatrix(i), field.logs_a, &field);
    field.addFC(wave.mainAlgorithm());

    cout << "Saved!" << endl;
    return true;
}

bool Controller::displayGraph(int i) {
    writeLog("Begin displayPoints");
    /* Print to the file all points and edges by binary matrix. */
    if (field.numBinMatrix() <= i) { return false; }
    field.drawBinGraph(i);
    cout << "Done! Saved!" << endl;
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

void Interface::writeLog(const string &message)
{
    /* Write log-message with date-time note. */
    logs << timeLog() << message << endl;
}

Interface::~Interface()
{
    logs.close();
    writeLog("DELETE");
}

void Interface::runCommand(string command)
{
    /* Get command from the user and compile it. */
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
            } else if (args.size() != 2) {throw - 1;}
            else {result = ctrl.preHist(args);}
        }

        else if (com == "SAVE") {
            if (args.size() == 0) { result = ctrl.printField(); }
            else { result = ctrl.printField(false, stod(args[0])); }
        }

        else if (com == "EXIT") {
            cout << "Okay..." << endl;
            return;
        } else if (com == "INFO") {
            result = ctrl.showInfoField();
        } else if (com == "INFOFC") {
            result = ctrl.showInfoFClusters();
        } else if ((com == "MATRIX") | (com == "ANALYSIS")) {
            result = ctrl.enterAnalysis();
        }

        else {
            cout << "No such command, sorry." << endl
                << "Would you like to get some HELP?" << endl;
        }
    }
    catch (...) {
        result = false;
        cout << "Somethig wrong, sorry.\nWould you like to get some HELP?" << endl;
    }
    if (result) {
        writeLog("\tCorrect <" + command + ">");
    } else {
        writeLog("\tIncorrect <" + command + ">");
    }
}