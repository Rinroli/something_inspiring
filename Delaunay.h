// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "Field.h"
#include "Triangle.h"
#include "Triangulation.h"

class Triangulation;
class Field;

#define PI 3.14159265

using namespace std;

// Temporaly struct for sorting points by slant.
struct tmp_int_double
{
    int point;
    double slant;
};

#if !defined(DELAUNAY)
#define DELAUNAY

class Delaunay
{
private:
    Triangulation* triangulation;
    ofstream& logs_a;
    Field* p_field;
    int step = 0;
    vector<vector<double>> phantom_points;

    vector<int> sortPolygon(set<int> polygon, int ind_poi);
    void createNewTriangles(vector<int> sorted, int ind_poi);
    Triangle findEnclosingTriangle();
    void saveStep(int cur_point, bool all_circ = false);
    void writeLog(const string& message);
public:
    Delaunay(Field* p_field, ofstream& logs_al);
    Triangulation* mainAlgorithm(); //  WARNING
};

#endif // DELAUNAY