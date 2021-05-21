// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>
#include <cmath>

#include "Triangle.h"
#include "Triangulation.h"
#include "my_functions.h"
#include "Point.h"
#include "Field.h"

class Point;
class Triangulation;

using namespace std;

#if !defined(PREDICTION)
#define PREDICTION

class Prediction
{
public:
    Prediction(vector<double> new_point, Triangulation* p_triangulation,
        Field* p_field, ofstream& logs_a);
    double predictPoint();
private:
    vector<double> findPlane();  // Ax + By + Cz + D = 0
    void writeLog(const string& message);
    vector<vector<double>> triangle_verts;
    vector<double> vert_val;
    ofstream& logs_a;
    vector<double> new_point;
};

#endif // PREDICTION