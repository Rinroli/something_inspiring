#include "Interface.h"

using namespace std;

#define N 1000
#define DELTA 0.1
#define K 10

Interface::Interface(vector<bool> if_logs, vector<string> name_logs)
    : ctrl(if_logs, name_logs) {
    if (if_logs[2]) {
        logs.open("logs/" + name_logs[2], ios_base::app);
        logs << endl << "New session" << endl;
        writeLog("INIT");
    }
}

Interface::~Interface()
{
    writeLog("DELETE");
    logs.close();
}

// Main loop for all program.
bool Interface::mainLoop() {
    ifstream fin;
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
        do {
            cout << "Command: ";
            getline(cin, command);
            runCommand(command);
        } while (command != "EXIT");
    }
    else {
        if (command == "") { command = "test.txt"; }

        if (command == "test") {
            writeLog("Test");
            string test_directory, test_name;
            cout << "Please enter test directory:\n > ";
            cin >> test_directory;
            cout << "And test name:\n > ";
            cin >> test_name;
            writeLog("\t" + test_directory + "/" + test_name + "/" + test_name + ".txt");

            fin.open("tests/" + test_directory + "/" + test_name + "/" + test_name + ".txt");
            output_directory = test_directory + "/" + test_name;
            output_name = test_name;

            ctrl.beginTest(output_directory, output_name);
        }
        else {
            writeLog("From File");
            fin.open("command_files/" + command);
        }

        if (!fin.is_open()) {
            cout << "Wrong file! Exit..." << endl;
            return 0;
        }
        getline(fin, command);
        while (!fin.eof() and (command == "" or runCommand(command))) {
            getline(fin, command);
        }
        cout << "End of file reached. Bye!" << endl;
    }
    return 1;
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
            if (args.size() == 0) { result = ctrl.genCloud(0, 0, 1, 1, N); }
            else if (args.size() < 4) { throw - 1; }
            else if (args.size() == 4) {
                result = ctrl.genCloud(stod(args[0]), stod(args[1]),
                    stod(args[2]), stod(args[3]), N);
            }
            else {
                result = ctrl.genCloud(stod(args[0]), stod(args[1]),
                    stod(args[2]), stod(args[3]), stod(args[4]));
            }
        }

        else if (com == "BINARY") {
            if (args.size() == 0) { result = ctrl.createIncMatrix(DELTA); }
            else { result = ctrl.createIncMatrix(stod(args[0])); }
        }

        else if (com == "DBSCAN") {
            if (args.size() == 0) { result = ctrl.createDBMatrix(DELTA, K); }
            else if (args.size() < 2) { throw - 1; }
            else { result = ctrl.createDBMatrix(stod(args[0]), stod(args[1])); }
        }

        else if ((com == "WAVE") | (com == "DBWAVE")) {
            if (args.size() == 0) { result = ctrl.waveClusters(-1); }
            else { result = ctrl.waveClusters(stod(args[0])); }
        }

        else if ((com == "DIBINARY") | (com == "DIDBSCAN") | (com == "DIB") | (com == "DID")) {
            if (args.size() == 0) { result = ctrl.displayGraph(-1); }
            else { result = ctrl.displayGraph(stod(args[0])); }
        }

        else if (com == "HIST") {
            if (args.size() == 0) {
                result = ctrl.saveHist();
            }
            else if (args.size() == 1) { throw - 1; }
            else { result = ctrl.preHist(args); }
        }

        else if (com == "KMEANS") {
            if (args.size() == 0) { result = ctrl.kMeans(25); }
            else { result = ctrl.kMeans(stod(args[0])); }
        }

        else if (com == "KERKMEANS") {
            if (args.size() == 0) { result = ctrl.kerKMeans(25, 5); }
            else { result = ctrl.kerKMeans(stod(args[0]), stod(args[1])); }
        }

        else if (com == "FOREL") {
            if (args.size() == 0) { result = ctrl.forelAlg(0.05); }
            else { result = ctrl.forelAlg(stod(args[0])); }
        }

        else if (com == "HIERARCH") {
            if (args.size() == 0) { result = ctrl.hierarchClustering(5); }
            else { result = ctrl.hierarchClustering(stoi(args[0])); }
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
            if (args.size() < 1) { throw - 1; }
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
        }
        else if (com == "INFO") {
            result = ctrl.showInfoField();
        }
        else if (com == "INFOFC") {
            result = ctrl.showInfoFClusters();
        }
        else if ((com == "MATRIX") | (com == "ANALYSIS")) {
            result = ctrl.enterAnalysis();
        }
        else if (com == "STREE") {
            result = ctrl.minSpanTree();
        }
        else if (com == "DELAUNAY") {
            result = ctrl.delaunayTriangulation();
        }
        else if (com == "STRHIST") {
            result = ctrl.streeHist();
        }
        else if (com == "FINDR") {
            result = ctrl.findR();
        }
        else if (com == "SHOWB") {
            result = ctrl.showBuffer();
        }
        else if (com == "PUTB") {
            result = ctrl.putBuffer();
        }
        else if (com == "EMPTYB") {
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
    return true;
}

// Write log-message with date-time note.
void Interface::writeLog(const string& message) {
    logs << timeLog() << message << endl;
}