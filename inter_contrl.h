// Copyright (c) 2020 Rinroli

#include "func_file.h"
// #include <string>
using namespace std;

#ifndef CONTROLLER
#define CONTROLLER

class Controller
{
public:
   explicit Controller(vector<bool> if_logs, vector<string> name_logs);
   ~Controller();
   bool showHelp();
   bool showInfoField();
   bool showInfoFClusters();
   bool showBuffer();
   bool addToBuffer(int ind);
   bool putBuffer();
   bool emptyBuffer();
   bool rotateBuffer(double alpha);
   bool moveBuffer(double x, double y);
   bool zoomBuffer(double k);
   bool printField(bool clouds, int i);
   bool saveHist();
   bool saveHist(Cluster cluster);
   bool streeHist();
   bool findR();
   bool kMeans(int n);
   bool kerKMeans(int k, int p);
   bool eMAlgorithm(int n);
   bool enterAnalysis();
   bool minSpanTree();
   bool createIncMatrix(double delta);
   bool createDBMatrix(double delta, int k);
   bool preHist(vector<string> args);
   bool genCloud(double mX, double mY, double sX, double sY,  int nu_points);
   bool waveClusters(int i);
   bool displayGraph(int i);
   void writeLog(const string& message);

private:
   Field field;
   ofstream logs;
};

#endif // CONTROLLER

#ifndef INTERFACE
#define INTERFACE

class Interface
{
public:
   explicit Interface(vector<bool> if_logs, vector<string> name_logs);
   ~Interface();
   bool runCommand(string command);
   void printConfigs();
   void changeConfigs();
   void writeConfigs(vector<vector<string>>);
   void writeLog(const string &command);

private:
   ofstream logs;
   Controller ctrl;
};

#endif // INTERFACE


///// CONFIGS /////

#if !defined(CONFIGS)
#define CONFIGS

class Configs
{
public:
   void printConfigs();
   void changeConfigs();
   void writeConfigs(vector<vector<string>> all_changes);
   vector<bool> if_logs{ true, true, true, true };  // f/c/i/a
   vector<string> name_logs{"", "", "", ""};
};

#endif // CONFIGS
