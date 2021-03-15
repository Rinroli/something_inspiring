// Copyright (c) 2020 Rinroli

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

#define INF 100000007

double determinant(const vector<vector<double>>& sigma);
double findDeterminant3(vector<vector<double>> matrix);
vector<vector<double>> eigen(const vector<vector<double>>& sigma);
vector<double> findEigenvalues(const vector<vector<double>>& sigma);

double getRandom();
int randInt(int max_val);
double distVectors(const vector<double>& vector_1,
    const vector<double>& vector_2);
double normalPoint(double mu, double sigma, int nu_points);
string timeLog();

class Field;
class Controller;
class Tree;
class Buffer;
class Forel;
class Triangulation;

#ifndef POINT
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
    friend Field;
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

double distPoints(Point f_poi, Point s_poi);
double distPoints(Point point, const vector<double>& center);
double distPoints(const vector<double>& center1, const vector<double>& center2);

#ifndef CLUSTER
#define CLUSTER

class Cluster
{
public:
    Cluster(int cur_id_cloud, ofstream& logs_field, Field* field);
    Cluster(int cur_id_cloud, ofstream& logs_field,
        Field* field, vector<Point>* p_field_points);
    Cluster(int id, const vector<int>& points, ofstream& logs_field, Field* field);
    Cluster(int id, const vector<int>& points, ofstream& logs_field,
        Field* field, vector<Point>* p_field_points);
    ~Cluster();
    int numPoints();
    int id;
    double getR();
    double getD();
    vector<double> getBox();
    vector<int> getCenter();
    void coutInfo();
    void printGnu(ofstream& file_out);
    Point& operator[](int i);
    Cluster& operator=(const Cluster& new_cluster);
    void clear();
    vector<double> findAverage();
    vector<Point>* p_field_points;
    friend Cluster& operator+=(Cluster& left, int i);
    friend Cluster& operator+=(Cluster& left, Point point);
    friend Cluster& operator+=(Cluster& left, Cluster& right);
    friend Buffer;
    friend Forel;

// protected:
    vector<int> id_points;
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
    friend Forel;
    friend Controller;

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
    Tree(Point& point, Tree* f_tree, Tree* s_tree, ofstream& logs_al);
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
    double dist_parent = -1;
    vector<Tree*> neighbors;
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


#if !defined(FOREL)
#define FOREL

class Forel
{
public:
    Forel(double R, const vector<Point>& real_pointss, Field* p_field,
        ofstream& logs_al, int g_stepp, int frame, int max_clusterss);
    ~Forel();
    vector<FindClusters> mainAlgorithm();
private:
    double R;
    vector<int> points;
    vector<Point> real_points;
    vector<Point> centroids;
    FindClusters f_clusters;
    vector<FindClusters> exported_points;
    Forel* centroid_clustering = nullptr;
    int step = 0, frame, global_step, max_clusters;
    Field* p_field;
    ofstream& logs_a;
    Cluster oneNewCluster();
    Cluster findNeighbourhood(const vector<double>& center);
    void removePoint(int i);
    Point& getPoint(int i);
    void exportPoints();
    void saveAnimation();
    void newFrame();
    void writeLog(const string& message);
};

#endif // FOREL


#if !defined(HIERARCH)
#define HIERARCH

class Hierarch
{
public:
    Hierarch(int k, Field* p_field, ofstream& ofstream);
    ~Hierarch();
    FindClusters mainAlgorithm();
private:
    vector<Tree*> current_trees;
    // vector<vector<int>> current_clusters;
    vector<Cluster> current_clusters;
    vector<vector<double>> dist_matrix;
    int k, num_clusters, step = 0, frame = 0;
    Field* p_field;
    ofstream& logs;
    vector<int> findMinDist();
    vector<int> recountDistMatrix(const vector<int>& find_ind);
    double getDist(int ind_1, int ind_2);
    void newFrame();
    void saveAnimation();
    void writeLog(const string& message);
};

#endif // HIERARCH



#ifndef FIELD
#define FIELD

class Field
{
public:
    Field(vector<bool> if_logs, vector<string> name_logs);
    ~Field();
    bool createCloud(double mX, double mY,
        double sX, double sY, int nu_points);
    bool createCloud(vector<Point> points);
    void print(ofstream& out_f);
    void print(int i, ofstream& out_f);
    void showBuffer();
    int numClouds();
    int numFClusters();
    int numPoints();
    double getDist(int ind1, int ind2);
    double getDist(Point point1, int ind2);
    vector<double> lineThroughPoints(int poi_1, int poi_2);
    vector<double> lineThroughPoints(vector<double> vpoi_1, int poi_2);
    Point& operator[](int i);
    FindClusters& getFCluster(int i);
    BinMatrix& getBinMatrix(int i);
    Point& getPoint(int i);
    Cloud& getCloud(int i);
    vector<double> getBox();

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
    void delaunayTriangulation();
    void binDBMatrix(double delta, int k);
    void binMatrix(double delta);
    void setAPoint(int i, int value);
    void addFC(FindClusters new_fc);
    void drawBinGraph(int i);
    void beginTest(const string& output_dir, const string& output_na);
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
    vector<double> containing_box{0, 0, 0, 0};  // {x_1, x_2, y_1, y_2}
    bool readonly;
    void updateD();
    void findBox();
    bool if_test = false;
    string output_name = "none";
    string output_directory = "data";
    vector<BinMatrix> bin_matrixes;
    Tree* p_tree = nullptr;
    Triangulation* p_triangulation = nullptr;
};

#endif // FIELD