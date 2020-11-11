// Copyright (c) 2020 Rinroli

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

#define INF 100000007

double determinant(const vector<vector<double>>& sigma);
vector<vector<double>> eigen(const vector<vector<double>>& sigma);
vector<double> findEigenvalues(const vector<vector<double>>& sigma);

double getRandom();
int randInt(int max_val);
double distVectors(const vector<double>& vector_1,
    const vector<double>& vector_2);
double normalPoint(double mu, double sigma);
string timeLog();

class Field;
class Controller;
class Tree;
class Buffer;

#ifndef POINT
#define POINT

class Point
{
public:
    Point(double mX, double mY, double sX, double sY,
        int idd, ofstream& logs_field, int id_cloudd);
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
    friend Field;
    int id;
    double x, y;
    int id_cloud;

private:
    int a, b;
    string mark = "base";
    ofstream& logs_f;
    void writeLog(const string& message);
};

#endif // POINT

double distPoints(Point f_poi, Point s_poi);
double distPoints(Point point, const vector<double>& center);
double distPoints(const vector<double>& center1, const vector<double>& center2);

#ifndef CLUSTER
#define CLUSTER

class Cluster
{
public:
    Cluster(int cur_id_cloud, ofstream& logs_field,
        Field* field);
    Cluster(int id, const vector<int>& points, ofstream& logs_field, Field* field);
    ~Cluster();
    int numPoints();
    int id;
    double getR();
    double getD();
    vector<double> getBox();
    vector<int> getCenter();
    void coutInfo();
    Point& operator[](int i);
    void clear();
    vector<double> findAverage();
    friend Cluster& operator+=(Cluster& left, int i);
    friend Buffer;

protected:
    vector<int> id_points;
    ofstream& logs_a;
    void writeLog(const string& message);
    bool updated = false;
    double R = INF, D = 0;
    vector<int> center;
    vector<double> box{ 0, 0, 0, 0 };
    void updateProp();
    vector<Point>* p_field_points;
    Field* p_field = nullptr;
};

#endif // CLUSTER

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
    friend Field;
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

#ifndef FIND_CLUSTERS
#define FIND_CLUSTERS

class FindClusters
{
public:
    FindClusters(ofstream& logs_al, string source);
    ~FindClusters();
    Cluster& operator[](int i);
    void coutInfo();
    int numClusters();
    void setID(int i);
    const string source;
    int id = 0;
    friend void operator+=(FindClusters& left, Cluster new_cl);

private:
    vector<Cluster> clusters;
    void writeLog(const string& message);
    ofstream& logs_a;
};

#endif // FIND_CLUSTERS

#ifndef BIN_MATRIX
#define BIN_MATRIX

class BinMatrix
{
public:
    BinMatrix(int size, double delta, int k = 1);
    ~BinMatrix();
    vector<bool>& operator[](int i);
    int size, k = 1;
    double delta;
    vector<int> marks;

private:
    vector<vector<bool>> matrix;
};

#endif // BIN_MATRIX


#if !defined(TREE)
#define TREE

class Tree {
public:
    Tree(Point& point, ofstream& logs_al, double dist = 0);
    ~Tree();
    void addVert(Point& point, double dist);
    void print(int indent = 0);
    int numTrees();
    vector<double> allDist();
    Tree operator[](int i);
    Tree* findIndex(int i);
    void displayTree(ofstream& out_f);

private:
    Point& point;
    double dist_parent = 0;
    vector<Tree> neighbors;
    void writeLog(const string& message);
    ofstream& logs_a;
};

#endif // TREE


#ifndef WAVE_CLUSTERS
#define WAVE_CLUSTERS

class WaveClusters
{
public:
    WaveClusters(const BinMatrix& matrix, ofstream& logs_field,
        Field* field);
    ~WaveClusters();
    FindClusters mainAlgorithm();

private:
    bool stepWave();
    void checkNeighbors(int i);
    bool findNextBegin();
    void writeLog(const string& message);
    vector<int> marks, step_points, sec_step_points;
    BinMatrix matrix_inc;
    int cur_step = 0;
    ofstream& logs_a;
    Field* p_field;
};

#endif // WAVE_CLUSTERS


#if !defined(KERKMEANS)
#define KERKMEANS

class KerKMeans
{
public:
    KerKMeans(int k, int p, Field& field, ofstream& logs_al);
    ~KerKMeans() {}
    FindClusters mainAlgorithm();
private:
    int p, k, number_poi, step = 0;
    Field& field;
    vector<vector<int>> clusters;
    vector<vector<vector<double>>> centers;
    bool pointDistribution();
    void newKernels();
    void saveStep();
    int nearestCenter(const Point& point);
    ofstream& logs_a;
    void writeLog(const string& message);
};

#endif // KERKMEANS

#if !defined(KMEANS)
#define KMEANS

class KMeans
{
public:
    KMeans(int k, Field& field, ofstream& logs_al);
    KMeans(int k, Field& field, const vector<int>& id_points, ofstream& logs_al);
    ~KMeans() {}
    FindClusters mainAlgorithm(bool silent=false);
    friend KerKMeans;
private:
    int k, number_poi, step = 1;
    Field& field;
    ofstream& logs_a;
    vector<vector<int>> clusters;
    void pointDistribution();
    int nearestCenter(const Point& point);
    bool findNewCenters();
    vector<double> getCoord(int ind_poi);
    void writeLog(const string& message);
    vector<vector<double>> centers;
    vector<int> id_points;
};

#endif // KMEANS

#if !defined(EMALGORITHM)
#define EMALGORITHM

class EMAlgorithm
{
public:
    EMAlgorithm(int k, Field& field, ofstream& logs_al);
    ~EMAlgorithm() {}
    FindClusters mainAlgorithm();
private:
    vector<vector<double>> point_clu_prob;
    vector<double> clu_prob, n_vect;
    vector<vector<double>> all_mu;
    vector<vector<vector<double>>> all_sigma;
    int k, step = 0, size_field;
    Field& field;
    ofstream& logs_a;
    double findPointProb(int p_j, int x_i);
    vector<vector<int>> findNearest();
    bool stepE();
    void stepM();
    double distMahalanobis(int p_j, int x_i);
    void saveStep();
    void writeLog(const string& message);
};

#endif // EMALGORITHM


#ifndef FIELD
#define FIELD

class Field
{
public:
    Field();
    ~Field();
    bool createCloud(double mX, double mY,
        double sX, double sY);
    bool createCloud(vector<Point> points);
    void print(ofstream& out_f);
    void print(int i, ofstream& out_f);
    void showBuffer();
    int numClouds();
    int numFClusters();
    int numPoints();
    double getDist(int ind1, int ind2);
    double getDist(Point point1, int ind2);
    Cloud& operator[](int i);
    FindClusters& getFCluster(int i);
    BinMatrix& getBinMatrix(int i);
    Point& getPoint(int i);
    bool addToBuffer(int i);
    bool putBuffer();
    bool emptyBuffer();
    bool rotateBuffer(double alpha);
    bool moveBuffer(double x, double y);
    bool zoomBuffer(double k);
    int numBinMatrix();
    bool ifReadonly();
    bool enterAnalysis();
    void minSpanTree();
    void binDBMatrix(double delta, int k);
    void binMatrix(double delta);
    void setAPoint(int i, int value);
    void addFC(FindClusters new_fc);
    void drawBinGraph(int i);
    vector<Point> points;
    vector<vector<double>> matrix;
    ofstream logs, logs_a;
    bool incMatrix(int k = 1);
    friend Controller;
private:
    vector<FindClusters> fclusters;
    vector<Cloud> clouds;
    Buffer buffer;
    void writeLog(const string& message);
    bool readonly;
    void updateD();
    vector<BinMatrix> bin_matrixes;
    Tree* p_tree = nullptr;
};

#endif // FIELD

