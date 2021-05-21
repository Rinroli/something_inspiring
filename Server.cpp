// Copyright (c) 2020 Rinroli

#include "Server.h"

Server::Server(vector<bool> if_logss, vector<string> name_logss) {
    p_ctrl = new Controller(if_logss, name_logss);
    logs.open("logs/logs_server.txt", ios_base::app);
    logs << endl << "New session" << endl;
    writeLog("INIT");

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        writeLog("cannot create socket");
        exit(EXIT_FAILURE);
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    err = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (err < 0) {
        writeLog("cannot bind socket");
        exit(EXIT_FAILURE);
    }

    err = listen(sock, 3);
    if (err < 0) {
        writeLog("listen queue failure");
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&active_set);
    FD_SET(sock, &active_set);

    writeLog("END INIT");
}

Server::~Server() {
    delete p_ctrl;
    a_delaunays.clear();
    a_ems.clear();
    a_forels.clear();
    a_hierarchs.clear();
    a_kerkmeanses.clear();
    a_kmeanses.clear();
    a_waves.clear();

    close(sock);
    writeLog("DELETE");
}

bool Server::startListen() {
    writeLog("Start listening: ", true);
    while (1) {
        read_set = active_set;
        if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0) {
            writeLog("Server: select  failure");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &read_set)) {
                if (i == sock) {
                    size = sizeof(client);
                    new_sock = accept(sock, (struct sockaddr*)&client, &size);
                    if (new_sock < 0) {
                        writeLog("Accept failure");
                        exit(EXIT_FAILURE);
                    }
                    writeLog("Connect from client #" + to_string(new_sock), true);
                    string if_test = readFromClient(new_sock);
                    if (if_test == "true") {
                        writeToClient(new_sock, "Wait for names");
                        startNewTest(new_sock);
                    }
                    FD_SET(new_sock, &active_set);
                }
                else {
                    if (not readParseAndAnswer(i)) {
                        return false;
                    }

                }
            }
        }
    }
}

bool Server::startNewTest(int socket) {
    writeLog("TEST");
    string o_dir = readFromClient(socket);
    writeToClient(socket, "Yes");
    string o_name = readFromClient(socket);
    writeToClient(socket, "Yes");
    string o_file = readFromClient(socket);
    writeToClient(socket, "Recieve all");
    writeLog("\t" + o_dir + " " + o_name + " " + o_file);

    p_ctrl->beginTest(o_dir, o_name, o_file);
    return true;
}

bool Server::readParseAndAnswer(int curr_sock) {
    writeLog("Start process, socket #" + to_string(curr_sock));
    string read_str = readFromClient(curr_sock);
    if (read_str == "") {
        close(curr_sock);
        FD_CLR(curr_sock, &active_set);
        return false;
    }
    else {
        if (read_str == "stop") {
            close(curr_sock);
            FD_CLR(curr_sock, &active_set);
            writeLog("Stop. Good Bye!", true);
            writeToClient(curr_sock, "Stop. Good Bye!");
            exit(0);
        }
        else if (read_str == "restart") {
            close(curr_sock);
            FD_CLR(curr_sock, &active_set);
            writeLog("Restart", true);
            writeToClient(curr_sock, "Restart");
            return false;
        }
        else {
            parseAndAnswer(curr_sock, read_str);
        }
    }
    return true;
}

bool Server::parseAndAnswer(int fd, string message) {
    writeLog("Start parseAndAnswer <" + message + ">");
    char* s = new char[message.size() + 1];
    strcpy(s, message.c_str());
    char* pch = strtok(s, ", ()");
    string com(pch);
    vector<string> args;
    while (pch != NULL) {
        pch = strtok(NULL, " ,()");
        if (pch == NULL) { break; }
        args.push_back(pch);
    }

    bool result = true;
    bool spec_message = true;

    if ((com == "GEN_CLOUD") | (com == "GC")) {
        result = p_ctrl->genCloud(stod(args[0]), stod(args[1]),
            stod(args[2]), stod(args[3]), stod(args[4]));
    }
    else if (com == "EXIT") {
        writeLog("Client asks to close connection <" + to_string(fd) + ">", true);
        close(fd);
        FD_CLR(fd, &active_set);
        writeToClient(fd, "Stop connection");
        return true;  
    }
    else if (com == "BINARY") { result = p_ctrl->createIncMatrix(stod(args[0])); }
    else if (com == "DBSCAN") { result = p_ctrl->createDBMatrix(stod(args[0]), stod(args[1])); }
    else if (com == "WAVE" or com == "DBWAVE") { result = p_ctrl->waveClusters(stod(args[0])); }
    else if ((com == "DIBINARY") | (com == "DIDBSCAN") | (com == "DIB") | (com == "DID") ) {
        result = p_ctrl->displayGraph(stod(args[0]));
    }
    else if (com == "HIST") {
        if (args.size() == 0) {
            result = p_ctrl->saveHist();
        } else { result = p_ctrl->preHist(args); }
    }
    else if (com == "KMEANS") { result = p_ctrl->kMeans(stod(args[0])); }
    else if (com == "KERKMEANS") { result = p_ctrl->kerKMeans(stod(args[0]), stod(args[1])); }
    else if (com == "FOREL") { result = p_ctrl->forelAlg(stod(args[0])); }
    else if (com == "HIERARCH") { result = p_ctrl->hierarchClustering(stod(args[0])); }
    else if (com == "EM") { result = p_ctrl->eMAlgorithm(stod(args[0])); }
    else if (com == "SAVE") {
        if (args.size() == 0) {
            result = p_ctrl->printField();
        } else {
            result = p_ctrl->printField(false, stod(args[0]));
        }
    }
    else if (com == "ADDB") { p_ctrl->addToBuffer(stod(args[0])); }
    else if (com == "ROTB") { p_ctrl->rotateBuffer(stod(args[0])); }
    else if (com == "MOVEB") { p_ctrl->moveBuffer(stod(args[0]), stod(args[1])); }
    else if (com == "ZOOMB") { p_ctrl->zoomBuffer(stod(args[0])); }
    else if (com == "INFO") { result = p_ctrl->showInfoField(); }
    else if (com == "INFOFC") { result = p_ctrl->showInfoFClusters(); }
    else if ((com == "MATRIX") | (com == "ANALYSIS")) { result = p_ctrl->enterAnalysis(); }
    else if (com == "STREE") { result = p_ctrl->minSpanTree(); }
    else if (com == "DELAUNAY") { result = p_ctrl->delaunayTriangulation(); }
    else if (com == "STRHIST") { result = p_ctrl->streeHist(); }
    else if (com == "FINDR") { result = p_ctrl->findR(); }
    else if (com == "SHOWB") { result = p_ctrl->showBuffer(); }
    else if (com == "PUTB") { result = p_ctrl->putBuffer(); }
    else if (com == "EMPTYB") { result = p_ctrl->emptyBuffer(); }
    else if (com == "PREDICT") { result = p_ctrl->pointPrediction(stod(args[0]), stod(args[1])); }

    writeLog("End step <" + to_string(fd) + ">");
    if (spec_message) {
        // cout << "<" << p_ctrl->curMessage() << ">" << endl;
        writeToClient(fd, p_ctrl->curMessage());
    } else {
        writeToClient(fd, message);
    }
    p_ctrl->cleanMessage();

    return result;
}

string Server::readFromClient(int fd) {
    int  nbytes;
    char buf[BUFLEN];

    nbytes = read(fd, buf, BUFLEN);

    if (nbytes < 0) {
        writeLog("Read failure");
        return "";
    }
    else if (nbytes == 0) {
        return "";
    }
    else {
        buf[nbytes] = 0;
        writeLog("Get '" + string(buf) + "'", true);
        return string(buf);
    }
}

// Send message to the client.
// DO NOT send send just a number!
bool Server::writeToClient(int fd, string message)
{
    if (message.size() < BUFLEN) {
        if (isNumber(message)) {
            writeLog("Numeric message!");
            return false;
        }
        int  nbytes;
        nbytes = write(fd, message.c_str(), message.size() + 1);
        writeLog("Write back: len " + to_string(message.size()));
        if (nbytes < 0) {
            writeLog("Write failure");
            return false;
        }
    } else {
        char buf[BUFLEN];
        int nu_packages = (message.size() / 511) + 1;
        string nu_p_str = to_string(nu_packages);
        writeLog("Very long message! In <" + nu_p_str + "> packages");
        int nbytes;
        write(fd, nu_p_str.c_str(), nu_p_str.size() + 1);

        for (int i = 0; i < nu_packages - 1; i++) {
            write(fd, (message.substr(i * 511, 511)).c_str(), 512);
            // cout << message.substr(i * 511, 511) << "\n\n";
            read(fd, buf, BUFLEN);
            writeLog("Get <" + string(buf) + "> from client");
        }
        write(fd, (message.substr((nu_packages - 1) * 511)).c_str(),
            message.size() - 511 * (nu_packages - 1) + 1);
        // cout << message.substr((nu_packages - 1) * 511) << "\n\n";
        read(fd, buf, BUFLEN);
        writeLog("Get <" + string(buf) + "> from client");
        writeLog("All sent");
    }
    return true;
}

// Write log-message with date-time note. 
void Server::writeLog(const string& message, bool cout_too)
{
    logs << timeLog() << "SERVER:\t" << message << endl;
    if (cout_too) {
        cout << "SERVER: " << message << endl;
    }
}
