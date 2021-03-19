// Copyright (c) 2020 Rinroli

#include "BinMatrix.h"

BinMatrix::BinMatrix(int sizee, double deltaa, int kk) {
    delta = deltaa;
    k = kk;
    size = sizee;
    matrix.resize(size);
    for (int i = 0; i < size; ++i) {
        matrix[i].resize(size);
    }
    marks = vector<int>(size);
}

BinMatrix::~BinMatrix() {
}

// Getter for matrix line.
vector<bool>& BinMatrix::operator[](int i) {
    return matrix[i];
}