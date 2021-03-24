// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>
#include <cstring>

#include "Controller.h" // WARNING
#include "my_functions.h"

class Controller;

using namespace std;

#ifndef INTERFACE
#define INTERFACE

class Interface
{
public:
    explicit Interface(vector<bool> if_logs, vector<string> name_logs);
    ~Interface();
    bool mainLoop();
    bool runCommand(string command);
    void printConfigs();
    void changeConfigs();
    void writeConfigs(vector<vector<string>>);
    void writeLog(const string& command);

private:
    ofstream logs;
    Controller *ctrl;
    bool if_test = false;
    string output_directory = "data";
    string output_name = "output.plt";
    string gen_file = "none";
    vector<bool> if_logs;
    vector<string> name_logs;
};

#endif // INTERFACE