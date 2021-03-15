// Copyright (c) 2020 Rinroli
// Version 2.0.1


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

    bool result;
    Interface interface(configs.if_logs, configs.name_logs);
    result = interface.mainLoop();

    return result;
}
