// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#include "my_functions.h"
// #include "Field.h"

#define PI 3.14159265

using namespace std;

#if !defined(POINT)
#define POINT

class Point
{
public:
    Point(double mX, double mY, double sX, double sY,
        int idd, ofstream& logs_field, int id_cloudd, int nu_points);
    Point(const vector<double>& coords, int idd, int id_cloud, ofstream& logs);
    explicit Point(ofstream& logs);
    void changeTo(Point point);
    void setID(int id);
    void setCoords(double x, double y);
    double getX();
    double getY();
    void rotatePoint(double alpha);
    void movePoint(double x, double y);
    void zoomPoint(double k);
    ~Point() {}
    vector<double> getCoord();
    void print(ofstream& out_f);
    friend ostream& operator<<(ostream& out, const Point& point);
    friend class Field;
    int id;
    double x, y;
    int id_cloud;

private:
    int a = -1, b = -1;
    string mark = "base";
    ofstream& logs_f;
    void writeLog(const string& message);
};

#endif // POINT