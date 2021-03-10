// Copyright (c) 2020 Rinroli

#pragma once

#include <iostream>
#include <vector>
#include <set>
#include "func_file.h"

using namespace std;

#if !defined(TRIANGLE)
#define TRIANGLE

class Triangle
{
private:
    vector<int> vert;
    vector<double> p_0, p_1, p_2;
    vector<double> center{0, 0};
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
    void print(ofstream& stream_tri, ofstream& stream_circ, bool all_circ=false);
    Triangle(vector<int> verts, Field* p_fieldd);
    Triangle(int vert_1, int vert_2, int vert_3, Field* p_fieldd);
    Triangle(vector<double> p_1, vector<double> p_2, vector<double> p_3, Field* p_fieldd);
    Triangle(vector<double> p_1, vector<double> p_2, vector<int> verts, 
        bool phantom, Field* p_fieldd);
};

#endif // TRIANGLE

class Triangulation
{
private:
    vector<Triangle> triangles;
    int nu_triangles = 0;
    Field* p_field;
    bool checkDelaunayCondition(Triangle& cur_tri, int ind_poi);
public:
    Triangulation(Field* p_fieldd): p_field(p_fieldd) {};
    void addTriangle(Triangle triangle);
    set<int> deleteTriangles(int ind_poi);
    void deleteEnclosingTriangle();
    int size();
    void printTriangle(int ind_tri, ofstream& stream_tri,
        ofstream& stream_circ, bool all_circ=false);
    void printLastCircle(ofstream& stream_circle);
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
    void saveStep(int cur_point, bool all_circ=false);
    void writeLog(const string& message);
public:
    Delaunay(Field* p_field, ofstream& logs_al);
    Triangulation* mainAlgorithm(); //  WARNING
};

#endif // DELAUNAY
