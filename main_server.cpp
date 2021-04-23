// Copyright (c) 2020 Rinroli
// Version 0.0.2

#include "Server.h"
#include "Configs.h"

using namespace std;

int main()
{
    srand(time(NULL));
    cout.setf(ios::boolalpha);
    cout.width(8);
    cout.precision(4);

    string command;
    Configs configs;
    // configs.printConfigs();

    // cout << "Do you want to change configs? (Y/(N)) ";
    // getline(cin, command);
    // if (command == "Y") {
    //     configs.changeConfigs();
    //     cout << "All changes will take effect immediately." << endl <<
    //         "Current configs are:" << endl << endl;
    //     configs.printConfigs();
    // }
    // else {
        configs.writeConfigs(vector<vector<string>>());
    // }
    cout << "Ready for requests >> " << endl;

    Server srv(configs.if_logs, configs.name_logs);
    srv.startListen();
    return 0;
}
  