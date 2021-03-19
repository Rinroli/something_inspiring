// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>
#include <set>

#include "Field.h"
#include "Triangle.h"

#include "my_functions.h"

class Field;
class Triangle;

using namespace std;

#if !defined(TRIANGULATION)
#define TRIANGULATION

class Triangulation
{
private:
    vector<Triangle> triangles;
    int nu_triangles = 0;
    Field* p_field;
    bool checkDelaunayCondition(Triangle& cur_tri, int ind_poi);
public:
    Triangulation(Field* p_fieldd) : p_field(p_fieldd) {};
    void addTriangle(Triangle triangle);
    set<int> deleteTriangles(int ind_poi);
    void deleteEnclosingTriangle();
    int size();
    void printTriangle(int ind_tri, ofstream& stream_tri,
        ofstream& stream_circ, bool all_circ = false);
    void printLastCircle(ofstream& stream_circle);
};

#endif // TRIANGULATION