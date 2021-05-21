// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>
#include <cstring>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// #include "Controller.h" // WARNING
#include "my_functions.h"

// class Controller;

using namespace std;

#define  SERVER_PORT     5559
#define  SERVER_NAME    "127.0.0.1"
#define  BUFLEN          512

#ifndef INTERFACE
#define INTERFACE

class Interface
{
public:
    explicit Interface(vector<bool> if_logs, vector<string> name_logs);
    ~Interface();
    bool mainLoop();
    void printConfigs();
    void changeConfigs();
    void writeConfigs(vector<vector<string>>);

private:
    ofstream logs;
    // Controller *ctrl;
    bool if_test = false;
    string output_directory = "data";
    string output_name = "output.plt";
    string gen_file = "none";
    vector<bool> if_logs;
    vector<string> name_logs;

    bool runCommand(string command);
    bool sendCommand(string command);
    bool readFromServer();
    void writeLog(const string& command, bool cout_too=false);

    int err;
    int sock;
    struct sockaddr_in server_addr;
    struct hostent* hostinfo;
};

#endif // INTERFACE