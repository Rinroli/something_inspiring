// Copyright (c) 2020 Rinroli

#include "func_file.h"
// #include <string>
using namespace std;

#ifndef CONTROLLER
#define CONTROLLER

class Controller
{
public:
   explicit Controller();
   ~Controller();
   bool showHelp();
   bool showInfoField();
   bool showInfoFClusters();
   bool printField(bool clouds, int i);
   bool saveHist();
   bool saveHist(Cluster cluster);
   bool streeHist();
   bool findR();
   bool enterAnalysis();
   bool minSpanTree();
   bool createIncMatrix(double delta);
   bool createDBMatrix(double delta, int k);
   bool preHist(vector<string> args);
   bool genCloud(double mX, double mY, double sX, double sY);
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
   explicit Interface(int idd = 0);
   ~Interface();
   bool runCommand(string command);
   void writeLog(const string &command);

private:
   int id;
   ofstream logs;
   Controller ctrl;
};

#endif // INTERFACE
