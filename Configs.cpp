// Copyright (c) 2020 Rinroli

#include <fstream>
#include <iostream>
#include <cstring>

#include "Configs.h"

using namespace std;

// Print all current configs to the standart output.
void Configs::printConfigs() {
    ifstream conf_input("config.txt");
    string readline;
    cout << "Current configs are: (what) / (if_write_logs) / (name_log_file)" << endl << endl;
    while (!conf_input.eof()) {
        getline(conf_input, readline);
        cout << readline << endl;
    }
    cout << endl;
    conf_input.close();
}

// Change configs from 'config.txt' file.
void Configs::changeConfigs() {
    vector<vector<string>> all_changes;

    cout << "What do you what to change? \nEnter f/c/i/a ---- field/controller/interface/algorithm" <<
        "\n(or exit for exit)" << endl << "> ";
    string command;
    vector<string> result_string;
    getline(cin, command);
    while (command != "exit") {
        if (command == "f") {
            result_string.push_back("FIELD");
        }
        else if (command == "c") {
            result_string.push_back("CONTROLLER");
        }
        else if (command == "i") {
            result_string.push_back("INTERFACE");
        }
        else if (command == "a") {
            result_string.push_back("ALGORITHM");
        }
        else {
            cout << "You're not going out until you do everything right." << endl <<
                "Well, you can still kill the script (everything will be allright), but this is shh-h-h." << endl <<
                "Be a little more careful. Let's start from the begining - f/c/i/a? " << endl << "> ";
            getline(cin, command);
            continue;
        }
        cout << "\nDo you want to write logs for it? (YES/NO)\nNothing will be 'YES'" << endl << "> ";
        getline(cin, command);
        if (command == "YES" or command == "NO") { result_string.push_back(command); }
        else if (command == "") { result_string.push_back("YES"); }
        else {
            cout << "You're not going out until you do everything right." << endl <<
                "Well, you can still kill the script (everything will be allright), but this is shh-h-h." << endl <<
                "Be a little more careful. Let's start from the begining - f/c/i/a? " << endl << "> ";
            getline(cin, command);
            continue;
        }

        if (command != "NO") {
            cout << "\nAnd now about filename." << endl <<
                "Enter a new name or leave it all blank - for using the old name." << endl <<
                "WARNING! .txt will be added automatically!" << endl << "> ";
            getline(cin, command);
            result_string.push_back(command + ".txt");
        }
        else { result_string.push_back(".txt"); }

        all_changes.push_back(result_string);
        result_string.clear();

        cout << "\nOne more time? \nf/c/i/a or exit\n> ";
        getline(cin, command);
    }

    writeConfigs(all_changes);
}

// Write all new changes to config.
// Objects can occur multiple times. Only the last entry will be counted.
void Configs::writeConfigs(vector<vector<string>> all_changes) {
    ifstream conf_input("config.txt");
    vector<vector<string>> all_configs;
    string readline;
    int cur_line = 0;
    while (cur_line < 4) {  // Standart split method, c++ style
        all_configs.push_back(vector <string>());
        getline(conf_input, readline);
        char* s = new char[readline.size() + 1];
        strcpy(s, readline.c_str());
        char* pch = strtok(s, ", ()");
        all_configs[cur_line].push_back(pch);
        while (pch != NULL) {
            pch = strtok(NULL, " ,()");
            if (pch == NULL) {
                break;
            }
            all_configs[cur_line].push_back(pch);
        }
        cur_line++;
    }
    conf_input.close();

    for (vector<string> entry : all_changes) {
        for (int ind_line = 0; ind_line < all_configs.size(); ind_line++) {
            if (all_configs[ind_line][0] == entry[0]) {
                if (entry[2] == ".txt") { entry[2] = all_configs[ind_line][2]; }
                all_configs[ind_line] = entry;
            }
        }
    }

    ofstream conf_output("config.txt");
    for (vector<string> line : all_configs) {
        conf_output << line[0] << " " << line[1] << " " << line[2] << endl;
        int cur_ob = 0;
        if (line[0] == "FIELD") {
            cur_ob = 0;
        }
        else if (line[0] == "CONTROLLER") {
            cur_ob = 1;
        }
        else if (line[0] == "INTERFACE") {
            cur_ob = 2;
        }
        else if (line[0] == "ALGORITHM") {
            cur_ob = 3;
        }

        if_logs[cur_ob] = (line[1] == "YES") ? true : false;
        name_logs[cur_ob] = line[2];
    }
    conf_output.close();
}
