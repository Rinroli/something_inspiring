// Copyright (c) 2020 Rinroli

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

#define INF 100000007

double getRandom();
double normalPoint(double mu, double sigma);
string timeLog();

class Field;
class Controller;

#ifndef POINT
#define POINT

class Point
{
public:
    Point(double mX, double mY, double sX, double sY,
          int idd, ofstream &logs_field, int id_cloudd);
    void changeTo(Point point);
    ~Point() {}
    vector<double> getCoord();
    void print(ofstream &out_f);
    friend ostream &operator<<(ostream &out, const Point &point);
    friend Field;
    int id;
    double x, y;
    int id_cloud;

private:
    int a, b;
    string mark = "base";
    ofstream &logs_f;
    void writeLog(const string &message);
};

#endif // POINT

double distPoints(Point f_poi, Point s_poi);

#ifndef CLUSTER
#define CLUSTER

class Cluster
{
public:
    Cluster(int cur_id_cloud, ofstream &logs_field,
            Field *field);
    ~Cluster();
    int numPoints();
    int id;
    double getR();
    double getD();
    vector<double> getBox();
    vector<int> getCenter();
    void coutInfo();
    Point operator[](int i);
    friend Cluster &operator+=(Cluster &left, int i);

protected:
    vector<int> id_points;
    ofstream &logs_a;
    void writeLog(const string &message);
    bool updated = false;
    double R = INF, D = 0;
    vector<int> center;
    vector<double> box{0, 0, 0, 0};
    void updateProp();
    vector<Point> *p_field_points;
    Field *p_field;
};

#endif // CLUSTER
// friend Cluster& operator+=(Cluster& left, int i);

#ifndef CLOUD
#define CLOUD

class Cloud : public Cluster
{
public:
    Cloud(int cur_id_cloud, ofstream &logs_field,
          Field *field);
    ~Cloud();

private:
    void writeLog(const string &message);
};

#endif // CLOUD

#ifndef FIND_CLUSTERS
#define FIND_CLUSTERS

class FindClusters
{
public:
    FindClusters(ofstream &logs_al, string source);
    ~FindClusters();
    Cluster &operator[](int i);
    void coutInfo();
    int numClusters();
    void setID(int i);
    const string source;
    int id = 0;
    friend void operator+=(FindClusters &left, Cluster new_cl);

private:
    vector<Cluster> clusters;
    void writeLog(const string &message);
    ofstream &logs_a;
};

#endif // FIND_CLUSTERS

#ifndef BIN_MATRIX
#define BIN_MATRIX

class BinMatrix
{
public:
    // BinMatrix(BinMatrix &bin_matrix);
    BinMatrix(int size, double delta, int k=1);
    ~BinMatrix();
    vector<bool>& operator[](int i);
    int size, k=1;
    double delta;
    vector<int> marks;

private:
    vector<vector<bool>> matrix;
};

#endif // BIN_MATRIX

#ifndef FIELD
#define FIELD

class Field
{
public:
    Field();
    ~Field();
    bool createCloud(double mX, double mY,
                     double sX, double sY);
    void print(ofstream &out_f);
    void print(int i, ofstream& out_f);
    int numClouds();
    int numFClusters();
    int numPoints();
    double getDist(int ind1, int ind2);
    Cloud &operator[](int i);
    FindClusters &getFCluster(int i);
    BinMatrix &getBinMatrix(int i);
    int numBinMatrix();
    bool ifReadonly();
    bool enterAnalysis();
    void binDBMatrix(double delta, int k);
    void binMatrix(double delta);
    void setAPoint(int i, int value);
    void addFC(FindClusters new_fc);
    void drawBinGraph(int i);
    int cur_id_points, cur_id_cloud;
    vector<Point> points;
    vector<vector<double>> matrix;
    ofstream logs, logs_a;
    bool incMatrix(int k = 1);
    friend Controller;
private:
    vector<FindClusters> fclusters;
    vector<Cloud> clouds;
    void writeLog(const string &message);
    bool readonly;
    int nextCloud();
    int nextPoint();
    void updateD();
    vector<BinMatrix> bin_matrixes;
    // void swapClusters(vector<Cluster> clu);
};

#endif // FIELD
