// Copyright (c) 2020 Rinroli

#pragma once

#include <iostream>

#include "Point.h"
// #include "Cluster.h"
#include "Field.h"
#include "my_functions.h"

class Field;
class Point;
class Cluster;

using namespace std;

#if !defined(BUFFER)
#define BUFFER

class Buffer
{
public:
    explicit Buffer(Field* field);
    ~Buffer();
    void coutInfo();
    friend void operator+=(Buffer& left, Cluster& new_cl);
    void putToField();
    bool isEmpty();
    friend class Field;
private:
    void writeLog(const string& message);
    void rotatePoints(double alpha);
    void movePoints(double x, double y);
    void zoomPoints(double k);
    void deletePoints();
    void addCluster(Cluster& new_cl);
    vector<Point> points;
    ofstream& logs_f;
    Field* p_field;
    int id = 1;
};

#endif // BUFFER