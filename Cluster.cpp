// Copyright (c) 2020 Rinroli

#include "Cluster.h"

Cluster::Cluster(int cur_id_cloud, ofstream& logs_field, Field* field)
    : logs_a{ logs_field }
{
    p_field_points = &(field->points);
    p_field = field;
    id = cur_id_cloud;
    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::Cluster(int cur_id_cloud, ofstream& logs_field, Field* field, vector<Point>* p_field_pointss)
    : logs_a{ logs_field }, p_field_points(p_field_pointss)
{
    p_field = field;
    id = cur_id_cloud;
    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::Cluster(int idd, const vector<int>& points, ofstream& logs_field, Field* field)
    : logs_a{ logs_field }, id_points(points)
{
    p_field_points = &(field->points);
    p_field = field;
    id = idd;

    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::Cluster(int idd, const vector<int>& points, ofstream& logs_field,
    Field* field, vector<Point>* p_field_pointss)
    : logs_a{ logs_field }, id_points(points), p_field_points(p_field_pointss)
{
    p_field = field;
    id = idd;

    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::~Cluster()
{
    id_points.clear();
    writeLog("DELETE");
}

// Return number of points in the cloud.
int Cluster::numPoints() {
    return id_points.size();
}

// Update Radius, Diameter and etc. of the Cluster(Cloud). 
void Cluster::updateProp() {
    if (updated) { return; }

    double tmp_dist;
    center.clear();
    for (int first : id_points) {
        double dist_f = 0;
        for (int second : id_points) {
            tmp_dist = p_field->getDist(first, second);
            if (tmp_dist > dist_f) { dist_f = tmp_dist; }
        }
        if (dist_f > D) { D = dist_f; }
        if (dist_f == R) { center.push_back(first); }
        if (dist_f < R) {
            center.clear();
            center.push_back(first);
            R = dist_f;
        }
    }

    if (numPoints() > 0) {   // Box
        vector<double> some_point = (*p_field_points)[id_points[0]].getCoord();

        box[0] = some_point[0];
        box[1] = some_point[0];
        box[2] = some_point[1];
        box[3] = some_point[1];

        for (int i = 0; i < numPoints(); ++i) {
            some_point = (*p_field_points)[id_points[i]].getCoord();
            if (some_point[0] > box[1]) { box[1] = some_point[0]; }
            if (some_point[0] < box[0]) { box[0] = some_point[0]; }
            if (some_point[1] > box[3]) { box[3] = some_point[1]; }
            if (some_point[1] < box[2]) { box[2] = some_point[1]; }
        }
    }
}

// Find average of coords.
vector<double> Cluster::findAverage() {
    vector<double> aver(2);
    for (int id_point : id_points) {
        vector<double> coords = (*p_field_points)[id_point].getCoord();
        aver[0] += coords[0];
        aver[1] += coords[1];
    }
    aver[0] /= numPoints();
    aver[1] /= numPoints();
    return aver;
}

// Getter for (R)adius.
double Cluster::getR() {
    updateProp();
    return R;
}

// Getter for (D)iameter.
double Cluster::getD() {
    updateProp();
    return D;
}

// Getter for the box containing the cluster.
vector<double> Cluster::getBox() {
    updateProp();
    return box;
}

// Getter for points from center of the cluster.
vector<int> Cluster::getCenter() {
    updateProp();
    return center;
}

// Getter for point by its index.
Point& Cluster::operator[](int i) {
    return (*p_field_points)[id_points[i]];
}

// Add point with absolut index i to the cluster.
Cluster& operator+=(Cluster& left, int i)
{
    left.id_points.push_back(i);
    return left;
}

// Add point with absolut index i to the cluster.
Cluster& operator+=(Cluster& left, Cluster& right)
{
    (left.id_points).insert((left.id_points).end(), (right.id_points).begin(),
        (right.id_points).end());
    return left;
}

Cluster& Cluster::operator=(const Cluster& new_cluster) {
    if (this == &new_cluster) {
        return *this;
    }
    id = new_cluster.id;
    id_points = new_cluster.id_points;
    updated = new_cluster.updated;
    R = new_cluster.R; D = new_cluster.D;
    center = new_cluster.center;
    box = new_cluster.box;
    p_field = new_cluster.p_field;
    p_field_points = new_cluster.p_field_points;
    return *this;
}

// Add point to the cluster.
Cluster& operator+=(Cluster& left, Point point)
{
    left.id_points.push_back(point.id);
    return left;
}

// Clear vector of points' id.
void Cluster::clear() {
    id_points.clear();
}

// Print info abput cluster.
void Cluster::coutInfo() {
    updateProp();
    cout << "\t\t#" << id << "  Points " << numPoints() << ";  Box: x:["
        << setw(8) << box[0] << ":"
        << setw(8) << box[1] << "] y:[" << setw(8) << box[2] << ":" << setw(8) 
        << box[3] << "]" << endl
        << "\t\t\t\tD = " << setw(8) << getD()
        << "\tR = " << setw(8) << getR()
        << "\t\tCenter: [";
    for (int i = 0; i < center.size(); ++i) {
        cout << (*p_field_points)[center[i]] << ", ";
    }
    cout << "]" << endl;
}

void Cluster::printGnu(ofstream& ofile) {
    for (int point_id : id_points) {
        (*p_field_points)[point_id].print(ofile);
    }
}

// Write log-message with date-time note.
void Cluster::writeLog(const string& message) {
    logs_a << timeLog() << "CLUSTER(id:" << id << "): " << message << endl;
}