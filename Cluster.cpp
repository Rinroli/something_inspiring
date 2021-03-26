// Copyright (c) 2020 Rinroli

#include "Cluster.h"

Cluster::Cluster(int cur_id_cloud, ofstream& logs_field, Field* field)
    : logs_a{ logs_field }
{
    p_field_points = &(field->points);
    p_field = field;
    indicator_points.resize(p_field->numPoints());
    id = cur_id_cloud;
    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::Cluster(int cur_id_cloud, ofstream& logs_field, Field* field, vector<Point>* p_field_pointss)
    : logs_a{ logs_field }, p_field_points(p_field_pointss)
{
    p_field = field;
    indicator_points.resize(p_field->numPoints());
    id = cur_id_cloud;
    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::Cluster(int idd, const vector<int>& points, ofstream& logs_field, Field* field)
    : logs_a{ logs_field }//, id_points(points)
{
    p_field_points = &(field->points);
    p_field = field;
    indicator_points.resize(p_field->numPoints());
    for (int point : points) {
        indicator_points[point] = 1;
    }
    id = idd;

    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::Cluster(int idd, const vector<double>& points_prob, ofstream& logs_field, Field* field)
    : logs_a{ logs_field }//, id_points(points)
{
    p_field_points = &(field->points);
    p_field = field;
    indicator_points.resize(p_field->numPoints());
    for (int id_point = 0; id_point < p_field->numPoints(); id_point++) {
        indicator_points[id_point] = points_prob[id_point];
    }
    id = idd;

    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::Cluster(int idd, const vector<int>& points, ofstream& logs_field,
    Field* field, vector<Point>* p_field_pointss)
    : logs_a{ logs_field }, p_field_points(p_field_pointss)//, id_points(points),
{
    p_field = field;
    indicator_points.resize(p_field->numPoints());
    for (int point : points) {
        indicator_points[point] = 1;
    }
    id = idd;

    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cluster::~Cluster()
{
    indicator_points.clear();
    writeLog("DELETE");
}

// Resize indicator vector if field has new points
void Cluster::resizeIndicator() {
    indicator_points.resize(p_field->numPoints());
}

// Return APPROXIMATE number of points in the cloud.
// Badly work for EM-clustering
int Cluster::apprNumPoints(double bound) {
    int count = 0;
    for (double ind_poi : indicator_points) {
        if (abs(ind_poi - bound) < EPS) {
            count++;
        }
    }
    return count;
}

// Return EXACT number of points in the cloud.
// Badly work for EM-clustering
int Cluster::numPoints() {
    return apprNumPoints(1);
}

// Return id of points that have indicator > bound
vector<int> Cluster::getPoints(double bound) {
    vector<int> res;
    for (int id_point = 0; id_point < p_field->numPoints(); id_point++) {
        if (indicator_points[id_point] - EPS > bound) {
            res.push_back(id_point);
        }
    }
    return res;
}

// Return one point id
int Cluster::getOnePoint(double bound) {
    for (int id_point : getPoints()) {
        if (indicator_points[id_point] - EPS > bound) {
            return id_point;
        }
    }
    return -1;
}

// Return one point coords
vector<double> Cluster::getOnePointCoords(double bound) {
    return p_field->getPoint(getOnePoint(bound)).getCoord();
}

// Update Radius, Diameter and etc. of the Cluster(Cloud). 
void Cluster::updateProp() {
    if (updated) { return; }

    double tmp_dist;
    center.clear();
    for (int first : getPoints()) {
        double dist_f = 0;
        for (int second : getPoints()) {
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
        vector<double> some_point(getOnePointCoords());

        box[0] = some_point[0];
        box[1] = some_point[0];
        box[2] = some_point[1];
        box[3] = some_point[1];

        for (int i : getPoints()) {
            some_point = p_field->getPoint(i).getCoord();
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
    for (int id_point : getPoints()) {
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

// // Getter for point by its index.
// Point& Cluster::operator[](int i) {
//     return (*p_field_points)[id_points[i]];
// }

// int Cluster::getIdPoint(int i) {
//     return id_points[i];
// }

// Add point with absolut index i to the cluster.
Cluster& operator+=(Cluster& left, int i)
{
    left.resizeIndicator();
    left.indicator_points[i] = 1;
    return left;
}

// Concatenate two clusters
Cluster& operator+=(Cluster& left, Cluster& right)
{
    // (left.id_points).insert((left.id_points).end(), (right.id_points).begin(),
    //     (right.id_points).end());
    left.resizeIndicator();
    for (int id_point = 0; id_point < right.p_field->numPoints(); id_point++) {
        left.indicator_points[id_point] += right.indicator_points[id_point];
        if (left.indicator_points[id_point] > 1) {
            left.indicator_points[id_point] = 0;
            left.writeLog("IS THAT HAPPENED?");
        }
    }
    return left;
}

Cluster& Cluster::operator=(const Cluster& new_cluster) {
    if (this == &new_cluster) {
        return *this;
    }
    id = new_cluster.id;
    indicator_points = new_cluster.indicator_points;
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
    // left.id_points.push_back(point.id);
    left.indicator_points[point.id] = 1;
    return left;
}

// Clear vector of points' id.
void Cluster::clear() {
    indicator_points.clear();
    indicator_points.resize(p_field->numPoints());
    // id_points.clear();
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
    for (int point_id : getPoints()) {
        p_field->getPoint(point_id).print(ofile);
    }
}

// Write log-message with date-time note.
void Cluster::writeLog(const string& message) {
    logs_a << timeLog() << "CLUSTER(id:" << id << "): " << message << endl;
}