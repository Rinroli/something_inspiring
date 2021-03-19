// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>

using namespace std;

#ifndef BIN_MATRIX
#define BIN_MATRIX

class BinMatrix
{
public:
    BinMatrix(int size, double delta, int k = 1);
    ~BinMatrix();
    vector<bool>& operator[](int i);
    int size, k = 1;
    double delta;
    vector<int> marks;

private:
    vector<vector<bool>> matrix;
};

#endif // BIN_MATRIX