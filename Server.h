// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>

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
#include <fcntl.h>

#include "Controller.h"
#include "my_functions.h"

class Controller;

using namespace std;

#define PORT    5555
#define BUFLEN  512

class Server
{
private:
    Controller* p_ctrl;
    void writeLog(const string& message, bool cout_too = false);

    vector<Delaunay> a_delaunays;
    vector<EMAlgorithm> a_ems;
    vector<Forel> a_forels;
    vector<Hierarch> a_hierarchs;
    vector<KerKMeans> a_kerkmeanses;
    vector<KMeans> a_kmeanses;
    vector<WaveClusters> a_waves;

    int     sock, new_sock;
    int     err, opt = 1;
    fd_set  active_set, read_set;
    struct  sockaddr_in  addr;
    struct  sockaddr_in  client;
    socklen_t  size;

    ofstream logs;
public:
    Server(vector<bool> if_logs, vector<string> name_logs);
    bool startListen();
    string readFromClient(int fds);
    bool readParseAndAnswer(int curr_sock);
    bool writeToClient(int fd, string message);
    bool parseAndAnswer(int fd, string message);
    ~Server();
};
