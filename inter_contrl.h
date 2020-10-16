// Copyright (c) 2020 Rinroli

#include "func_file.h"
// #include <string>
using namespace std;

#ifndef CONTROLLER
#define CONTROLLER

class Controller
{
public:
   explicit Controller(bool iflogg = 0, string log_name = "logs_ctrl.txt");
   ~Controller();
   bool showHelp();
   bool showInfoField();
   bool showInfoFClusters();
   bool printField(bool clouds, int i);
   bool saveHist();
   bool saveHist(Cluster cluster);
   void writeLog(string message);
   bool enterAnalysis();
   bool createIncMatrix(double delta);
   bool createDBMatrix(double delta, int k);
   bool preHist(vector<string> args);
   bool genCloud(double mX, double mY, double sX, double sY);
   bool waveClusters(int i);
   bool displayGraph(int i);

private:
   bool iflog;
   Field field;
   ofstream logs;
};

#endif // CONTROLLER

#ifndef INTERFACE
#define INTERFACE

class Interface
{
public:
   explicit Interface(bool iflogg = 0, string log_namee = "logs/logs_interface.txt",
                      int idd = 0, int nu_of_logff = 0);
   ~Interface();
   void runCommand(string command);
   void writeLog(string command);

private:
   int id, nu_of_logf;
   bool iflog;
   string log_name;
   ofstream logs;
   Controller ctrl;
};

#endif // INTERFACE
