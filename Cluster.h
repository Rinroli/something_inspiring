// Copyright (c) 2020 Rinroli

#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "Point.h"
#include "Field.h"

#ifndef CLUSTER
#define CLUSTER

#define INF 100000007

class Field;
class Point;
// class Cloud;
class Buffer;

class Cluster
{
public:
    Cluster(int cur_id_cloud, ofstream& logs_field, Field* field);
    Cluster(int cur_id_cloud, ofstream& logs_field,
        Field* field, vector<Point>* p_field_points);
    Cluster(int id, const vector<int>& points, ofstream& logs_field, Field* field);
    Cluster(int id, const vector<double>& points, ofstream& logs_field, Field* field);
    Cluster(int id, const vector<int>& points, ofstream& logs_field,
        Field* field, vector<Point>* p_field_points);
    ~Cluster();
    void resizeIndicator();
    int numPoints();
    int apprNumPoints(double bound=0.5);
    vector<int> getPoints(double bound=0.5);
    int id;
    double getR();
    double getD();
    vector<double> getBox();
    vector<int> getCenter();
    int getOnePoint(double bound=0.5);
    vector<double> getOnePointCoords(double bound=0.5);
    void coutInfo();
    void printGnu(ofstream& file_out);
    // Point& operator[](int i);
    // int getIdPoint(int i);
    Cluster& operator=(const Cluster& new_cluster);
    void clear();
    vector<double> findAverage();
    vector<Point>* p_field_points;
    friend Cluster& operator+=(Cluster& left, int i);
    friend Cluster& operator+=(Cluster& left, Point point);
    friend Cluster& operator+=(Cluster& left, Cluster& right);
    friend class Buffer;
    friend class Forel;

protected:
    // vector<int> id_points;
    vector<double> indicator_points;
    ofstream& logs_a;
    void writeLog(const string& message);
    bool updated = false;
    double R = INF, D = 0;
    vector<int> center;
    vector<double> box{ 0, 0, 0, 0 };
    void updateProp();
    Field* p_field = nullptr;
};

#endif // CLUSTER


#ifndef CLOUD
#define CLOUD

class Cloud : public Cluster
{
public:
    Cloud(int cur_id_cloud, ofstream& logs_field,
        Field* field);
    Cloud(int id, const vector<int>& points,
        ofstream& logs_field, Field* field);
    ~Cloud();

private:
    void writeLog(const string& message);
};

#endif // CLOUD