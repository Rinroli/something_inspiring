// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <string>

using namespace std;

#if !defined(CONFIGS)
#define CONFIGS

class Configs
{
public:
    void printConfigs();
    void changeConfigs();
    void writeConfigs(vector<vector<string>> all_changes);
    vector<bool> if_logs{ true, true, true, true };  // f/c/i/a
    vector<string> name_logs{ "", "", "", "" };
};

#endif // CONFIGS
