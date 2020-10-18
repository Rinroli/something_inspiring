// Copyright (c) 2020 Rinroli
// Version 1.4.3


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

    Interface interface;
    string command;

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
