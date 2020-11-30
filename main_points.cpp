// Copyright (c) 2020 Rinroli
// Version 1.8.0


#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
// #include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <queue>

#include "func_file.h"

using namespace std;

#include "func_file.h"
#include "inter_contrl.h"


int main() {
    srand(time(NULL));
    cout.setf(ios::boolalpha);
    cout.width(8);
    cout.precision(4);

    string command;
    Configs configs;
    configs.printConfigs();

    cout << "Do you want to change configs? (Y/(N)) ";
    getline(cin, command);
    if (command == "Y") {
        configs.changeConfigs();
        cout << "All changes will take effect immediately." << endl <<
            "Current configs are:" << endl << endl;
        configs.printConfigs();
    } else {
        configs.writeConfigs(vector<vector<string>>());
    }

    Interface interface(configs.if_logs, configs.name_logs);

    ifstream fin;

    cout << "Would you like to CLI or command file?" << endl << 
        "If CLI put cin, else put file name." << 
        "\n\tIt should be in command_files directory. By default, command is 'test.txt'" <<
        endl << "Your choice: ";
    getline(cin, command);
    if (command == "cin") {
        do {
            cout << "Command: ";
            getline(cin, command);
            interface.runCommand(command);
        } while (command != "EXIT");
    }
    else {
        if (command == "") {command = "test.txt";}
        fin.open("command_files/" + command);
        if (!fin.is_open()) {
            cout << "Wrong file! Exit..." << endl;
            return 0;
        }
        getline(fin, command);
        while (interface.runCommand(command)) {
            getline(fin, command);
        }
    }

    return 0;
}
