// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>

#include "Field.h"
#include "Point.h"

class Field;

using namespace std;

#if !defined(TRIANGLE)
#define TRIANGLE

class Triangle
{
private:
    vector<int> vert;
    vector<double> p_0, p_1, p_2;
    vector<double> center{ 0, 0 };
    double radius;
    Field* p_field;
    bool phantom = false;
    void findCircle();
    double findA();
    double findB();
    double findC();
public:
    vector<double> getCenter();
    double getRadius();
    bool isPhantom();
    vector<int> getVerts();
    bool containPoint(vector<double>);
    void print(ofstream& stream_tri, ofstream& stream_circ, bool all_circ = false);
    Triangle(vector<int> verts, Field* p_fieldd);
    Triangle(int vert_1, int vert_2, int vert_3, Field* p_fieldd);
    Triangle(vector<double> p_1, vector<double> p_2, vector<double> p_3, Field* p_fieldd);
    Triangle(vector<double> p_1, vector<double> p_2, vector<int> verts,
        bool phantom, Field* p_fieldd);
};

#endif // TRIANGLE