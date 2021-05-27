#include "Interface.h"

using namespace std;

#define N 1000
#define DELTA 0.1
#define K 10

Interface::Interface(vector<bool> if_logss, vector<string> name_logss)
: if_logs(if_logss), name_logs(name_logss) {
    if (if_logs[2]) {
        logs.open("logs/" + name_logs[2], ios_base::app);
        logs << endl << "New session" << endl;
        writeLog("INIT");
    }

    hostinfo = gethostbyname(SERVER_NAME);
    if (hostinfo == NULL) {
        writeLog("Unknown host " + string(SERVER_NAME), true);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr = *(struct in_addr*)hostinfo->h_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        writeLog("Client: socket was not created", true);
        exit(EXIT_FAILURE);
    }

    err = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (err < 0) {
        writeLog("Client:  connect failure", true);
        exit(EXIT_FAILURE);
    }
    writeLog("Connection is ready", true);
}

Interface::~Interface()
{
    writeLog("DELETE");
    close(sock);
    logs.close();
}

// Main loop for all program.
bool Interface::mainLoop() {
    writeLog("Start mainLoop");
    ifstream fin, gen_fin;
    string command;

    cout << "Would you like to CLI or command file?" << endl <<
        " > If CLI put cin, else put file or test name." <<
        "\n > Command file should be in command_files directory. By default, command is 'test.txt'" <<
        endl << " > If you want to run test enter 'test' then 'test_type' and 'test_name.txt'." <<
        " Test should be in test directory and WITHOUT '.txt'!\n > The result will be generated in the same directory." <<
        endl << "Your choice: ";
    getline(cin, command);
    if (command == "cin") {
        writeLog("Command Line Interface");
        sendCommand("false");
        do {
            cout << colorString("Command: ", "cyan");
            getline(cin, command);
            runCommand(command);
            if (command != "EXIT") { readFromServer(); }
        } while (command != "EXIT");
    }
    else {
        if (command == "") { command = "test.txt"; }

        if (command == "test") {
            writeLog("Test");
            string test_directory, test_name, cur_gen;
            cout << "Please enter test directory:\n > ";
            cin >> test_directory;
            cout << "And test name:\n > ";
            cin >> test_name;
            writeLog("\t" + test_directory + "/" + test_name + "/" + test_name + ".txt");

            output_directory = test_directory + "/" + test_name;
            output_name = test_name;
    
            fin.open("tests/" + test_directory + "/" + test_name + "/" + test_name + ".txt");
            if (test_directory == "Algorithm") {
                cout << "At last - generation file:\n > ";
                cin >> cur_gen;
                gen_file = "tests/Generation/" + cur_gen;
                sendCommand("true");
                readFromServer();
                sendCommand(output_directory);
                readFromServer();
                sendCommand(output_name);
                readFromServer();
                sendCommand(gen_file);
                readFromServer();
                writeLog("\tGen file >> " + gen_file + "/" + cur_gen + ".txt");
                gen_fin.open(gen_file + "/" + cur_gen + ".txt");

                if (!gen_fin.is_open()) {
                    cout << "Wrong generation file! Exit..." << endl;
                    return 0;
                }
            } else {
                gen_file = "tests/Generation/" + cur_gen;
                sendCommand("true");
                readFromServer();
                sendCommand(output_directory);
                readFromServer();
                sendCommand(output_name);
                readFromServer();
                sendCommand(gen_file);
                readFromServer();
                writeLog("\tGen file >> " + gen_file + "/" + cur_gen + ".txt");
            }
        }
        else {
            writeLog("From File");
            sendCommand("false");
            fin.open("command_files/" + command);
        }

        if (!fin.is_open()) {
            cout << "Wrong file! Exit..." << endl;
            return 0;
        }

        if (command == "test") {
                getline(gen_fin, command);
                while (!gen_fin.eof() and (command == "" or runCommand(command))) {
                    readFromServer();
                    getline(gen_fin, command);
                    break;
                }
                gen_fin.close();
                writeLog("RETURN to test file");
                getline(fin, command);
                while (!fin.eof() and (command == "" or runCommand(command))) {
                    getline(fin, command);
                    if (command != "EXIT") { readFromServer(); }
                }
        } else {
            getline(fin, command);
            while (!fin.eof() and (command == "" or runCommand(command))) {
                if (command != "EXIT") { readFromServer(); }
                getline(fin, command);
            }
        }
        cout << "End of file reached. Bye!" << endl;
    }
    return 1;
}

// Send command to the server.
bool Interface::sendCommand(string command) {
    writeLog("Start sendCommand <" + command + "> " + to_string(command.size()));
    int nbytes = write(sock, command.c_str(), command.size() + 1);
    if (nbytes < 0) { writeLog("Write error"); return false; }
    if (strstr(command.c_str(), "stop")) return false;
    writeLog("\tEnd sendCommand");
    return true;
}

// Recive commnd from the server.
bool Interface::readFromServer()
{
    writeLog("Start readFromServer");
    int   nbytes;
    char  buf[BUFLEN];
    char spec_buf[BUFLEN];

    nbytes = read(sock, buf, BUFLEN);
    if (nbytes < 0) {  // error
        writeLog("Error", true);
        return -1;
    }
    else if (nbytes == 0) {  // no data
        writeLog("Server: no message", true);
    }
    else {  // success
        if (not isNumber(string(buf))) {
            cout << "Server's replay: " + string(buf) << endl;
            writeLog("Receive message, len <" + to_string(nbytes - 1) + ">");
        } else {
            string get_str = "Get";
            int nu_packages = stod(string(buf));
            for (int i = 0; i < nu_packages; i++) {
                nbytes = read(sock, spec_buf, BUFLEN);

                cout << string(spec_buf);
                if (i < nu_packages - 1) { cout << " " + colorString("|<-->|", "red", true) + " "; }
                writeLog("Receive package #" + to_string(i) + " len <" +
                    to_string(string(spec_buf).size()) + "> !! <" + to_string(nbytes) + ">");

                write(sock, (get_str + " " + to_string(i)).c_str(), 
                    (get_str + " " + to_string(i)).size() + 1);
                writeLog("\tSend 'get'");
            }
        }
    }
    return 0;
}

// Get command from the user and compile it.
bool Interface::runCommand(string command)
{
    writeLog("GET COMMAND <" + command + ">");
    cout << colorString(command + ": ", "blue", true);

    bool result = true;

    char* s = new char[command.size() + 1];
    strcpy(s, command.c_str());
    char* pch = strtok(s, ", ()");
    string com(pch);
    vector<string> args;

    try
    {
        while (pch != NULL) {
            pch = strtok(NULL, " ,()");
            if (pch == NULL) {
                break;
            }
            args.push_back(pch);
        }

        string result_command(com);
        for (string arg_tmp : args) {
            result_command += " " + arg_tmp;
        }

        if (command == "HELP") {
            writeLog("Begin showHelp");
            ifstream help_file("help.txt");
            string readline;
            while (!help_file.eof()) {
                getline(help_file, readline);
                cout << readline << endl;
            }
            help_file.close();
            writeLog("\tEnd showHelp");
        }

        else if (com == "EXIT") {
            cout << "Okay!.." << endl;
            sendCommand("EXIT");
            readFromServer();
            writeLog("\tCorrect <" + command + ">");
            return false;
        }

        else if ((com == "GEN_CLOUD") | (com == "GC")) {
            if (args.size() == 0) { result_command = "GC 0 0 1 1 " + to_string(N); }
            else if (args.size() < 4 or args.size() > 5) { throw - 1; }
            else {
                result_command = "GC";
                for (int nu_a = 0; nu_a < 5; nu_a++) {
                    if (args.size() > nu_a) {
                        result_command += " " + args[nu_a];
                    } else {
                        if (nu_a < 2) { result_command += " 0"; }
                        else if (nu_a < 4) { result_command += " 1"; }
                        else {result_command += " " + to_string(N); }
                    }
                }
            }

        }

        else if (com == "BINARY") {
            if (args.size() == 0) { result_command += " " + to_string(DELTA); }
            else if (args.size() > 1) { throw - 1; }
        }

        else if (com == "DBSCAN") {
            if (args.size() == 0) { result_command += " " + to_string(DELTA) + " " + to_string(K); }
            else if (args.size() < 2 or args.size() > 2) { throw - 1; }
        }

        else if ((com == "WAVE") | (com == "DBWAVE")) {
            if (args.size() == 0) { result_command = "DBWAVE -1"; }
            else if (args.size() > 1) { throw - 1; }
        }

        else if ((com == "DIBINARY") | (com == "DIDBSCAN") | (com == "DIB") | (com == "DID")) {
            if (args.size() == 0) { result_command = "DIBINARY -1"; }
            else if (args.size() > 1) { throw - 1; }
        }

        else if (com == "HIST") {
            if (args.size() == 0) { result_command = "HIST"; }
            else if (args.size() == 1) { throw - 1; }
            else if (args.size() > 3) { throw - 1; }
        }

        else if (com == "PREDICT") {
            if (args.size() != 2 and args.size() != 0) { throw -1; }
        }

        else if (com == "KMEANS") {
            if (args.size() == 0) { result_command += " 25"; }
            else if (args.size() > 1) { throw - 1; }
        }

        else if (com == "KERKMEANS") {
            if (args.size() == 0) { result_command += " 25 5"; }
            else if (args.size() > 1) { throw - 1; }
        }

        else if (com == "FOREL") {
            if (args.size() == 0) { result_command += " 0.05"; }
            else if (args.size() > 1) { throw - 1; }
        }

        else if (com == "HIERARCH") {
            if (args.size() == 0) { result_command += " 5"; }
            else if (args.size() > 1) { throw - 1; }
        }

        else if (com == "EM") {
            if (args.size() == 0) { result_command += " 25"; }
            else if (args.size() > 1) { throw - 1; }
        }

        else if (com == "SAVE") {
            if (args.size() > 1) { throw - 1; }
        }

        else if (com == "ADDB") {
            if (args.size() == 0) { result_command += " -1"; }
            else if (args.size() > 1) { throw - 1; }
        }

        else if (com == "ROTB") {
            if (args.size() < 1 or args.size() > 1) { throw - 1; }
        }

        else if (com == "MOVEB") {
            if (args.size() < 2 or args.size() > 2) { throw - 1; }
        }

        else if (com == "ZOOMB") {
            if (args.size() < 1 or args.size() > 1) { throw - 1; }
        }

        else if (com == "INFO" or com == "INFOFC" or com == "MATRIX" or com == "ANALYSIS"
            or com == "STREE" or com == "DELAUNAY" or com == "STRHIST" or com == "FINDR"
            or com == "SHOWB" or com == "PUTB" or com == "EMPTYB" or com == "stop"
            or com == "restart") {
                if (args.size() > 1) { throw - 1; }
        }

        else {
            cout << "No such command, sorry." << endl
                << "Would you like to get some HELP?" << endl;
        }

        sendCommand(result_command);
    }
    catch (int i) {
        result = false;
        if (i == -1) {
            cout << "Wrong command."
                << "nWould you like to get some HELP?" << endl;
        }
        else if (i == -2) {
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
        cout << "Something wrong, sorry. "
            << "Would you like to get some HELP?" << endl;
    }
    if (result) { writeLog("\tCorrect <" + command + ">"); }
    else { writeLog("\tIncorrect <" + command + ">"); }
    if (command == "stop") {  // Yeah, crutch! 
        cout << "Okay..." << endl;
        writeLog("\tCCorrect <" + command + ">");
        return false;
    }
    return true;
}

// Write log-message with date-time note.
void Interface::writeLog(const string& message, bool cout_too) {
    logs << timeLog() << message << endl;
    if (cout_too) {
        cout << message << endl;
    }
}