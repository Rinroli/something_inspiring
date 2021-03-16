// Copyright (c) 2020 Rinroli

#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>

#include "neuron.h"
#include "func_file.h"

#define PI 3.14159265

using namespace std;


///// Trinagle /////

Triangle::Triangle(vector<int> verts, Field* p_fieldd)
    : p_field(p_fieldd), vert(verts) {
    p_0 = (p_field->getPoint(vert[0])).getCoord();
    p_1 = (p_field->getPoint(vert[1])).getCoord();
    p_2 = (p_field->getPoint(vert[2])).getCoord();
    findCircle();
}

Triangle::Triangle(int vert_1, int vert_2, int vert_3, Field* p_fieldd)
    : p_field(p_fieldd) {
    vert.push_back(vert_1); vert.push_back(vert_2); vert.push_back(vert_3);
    p_0 = (p_field->getPoint(vert[0])).getCoord();
    p_1 = (p_field->getPoint(vert[1])).getCoord();
    p_2 = (p_field->getPoint(vert[2])).getCoord();
    findCircle();
}

Triangle::Triangle(vector<double> p_11, vector<double> p_22, vector<int> verts, bool ph, Field* p_fieldd)
    : p_0(p_11), p_1(p_22), vert(verts), phantom(ph), p_field(p_fieldd) {
    p_2 = (p_field->getPoint(vert[2])).getCoord();
    findCircle();
}

Triangle::Triangle(vector<double> p_11, vector<double> p_22, vector<double> p_33, Field* p_fieldd)
    : p_0(p_11), p_1(p_22), p_2(p_33), p_field(p_fieldd),
    vert(vector<int> {-1, -2, -3}) {
    phantom = true;
    findCircle();
}

// Find center and radius of the circle.
// Circle equation (x^2 + y^2)*a - x*b + y*c - d = 0
void Triangle::findCircle() {
    double a = findA();
    double b = findB();
    double c = findC();

    center[0] = b / (2 * a);
    center[1] = (-c) / (2 * a);

    radius = distPoints(center, p_1);
}

// Retern coords of the center.
vector<double> Triangle::getCenter() {
    return center;
}

// Retern coords of the radius.
double Triangle::getRadius() {
    return radius;
}

// Return if triangle is phantom.
bool Triangle::isPhantom() {
    return phantom;
}

// Return vertexes of the triangle.
vector<int> Triangle::getVerts() {
    return vert;
}

// Print all adges to the stream.
void Triangle::print(ofstream& stream_tri, ofstream& stream_circ, bool all_circ) {
    stream_tri << endl << p_0[0] << " " << p_0[1] << endl;
    stream_tri << p_1[0] << " " << p_1[1] << endl;
    stream_tri << p_2[0] << " " << p_2[1] << endl;
    stream_tri << p_0[0] << " " << p_0[1] << endl << endl;

    if (all_circ) {
        stream_circ << center[0] << " " << center[1] << " " << radius << endl << endl;
    }
}

// Find coefficient a of the circle equation.
double Triangle::findA() {
    vector<vector<double>> matrix;
    matrix.push_back(vector<double> {p_0[0], p_0[1], 1});
    matrix.push_back(vector<double> {p_1[0], p_1[1], 1});
    matrix.push_back(vector<double> {p_2[0], p_2[1], 1});
    return findDeterminant3(matrix);
}

// Find coefficient b of the circle equation.
double Triangle::findB() {
    vector<vector<double>> matrix;
    matrix.push_back(vector<double> {p_0[0] * p_0[0] + p_0[1] * p_0[1], p_0[1], 1});
    matrix.push_back(vector<double> {p_1[0] * p_1[0] + p_1[1] * p_1[1], p_1[1], 1});
    matrix.push_back(vector<double> {p_2[0] * p_2[0] + p_2[1] * p_2[1], p_2[1], 1});
    return findDeterminant3(matrix);
}

// Find coefficient c of the circle equation.
double Triangle::findC() {
    vector<vector<double>> matrix;
    matrix.push_back(vector<double> {p_0[0] * p_0[0] + p_0[1] * p_0[1], p_0[0], 1});
    matrix.push_back(vector<double> {p_1[0] * p_1[0] + p_1[1] * p_1[1], p_1[0], 1});
    matrix.push_back(vector<double> {p_2[0] * p_2[0] + p_2[1] * p_2[1], p_2[0], 1});
    return findDeterminant3(matrix);
}


///// Triangulation /////

// Delete triangles that don't satisfy the Delaunay condition.
// Return vector of points of these triangles.
set<int> Triangulation::deleteTriangles(int ind_poi) {
    set<int> polygon_unsorted;
    for (int ind_tri = 0; ind_tri < nu_triangles; ind_tri++) {
        if (checkDelaunayCondition(triangles[ind_tri], ind_poi)) {
            vector<int> verts = triangles[ind_tri].getVerts();
            polygon_unsorted.insert(verts[0]);
            polygon_unsorted.insert(verts[1]);
            polygon_unsorted.insert(verts[2]);

            triangles.erase(triangles.begin() + ind_tri);
            ind_tri--;
            nu_triangles--;
        }
    }
    return polygon_unsorted;
}

// Check Delaunay conditions for triangle and point.
bool Triangulation::checkDelaunayCondition(Triangle& tri, int ind_poi) {
    return distPoints((p_field)->getPoint(ind_poi), tri.getCenter()) < tri.getRadius();
}

// Delete phantom enclosing triangle and connected triangles.
void Triangulation::deleteEnclosingTriangle() {
    for (int ind_tri = 0; ind_tri < nu_triangles; ind_tri++) {
        if (triangles[ind_tri].isPhantom()) {
            triangles.erase(triangles.begin() + ind_tri);
            ind_tri--;
            nu_triangles--;
        }
    }
}

// Simple addition of new triangle to Triangulation.
void Triangulation::addTriangle(Triangle triangle) {
    triangles.push_back(triangle);
    nu_triangles++;
}

// Print all edges of trinagle (by index) to the stream.
void Triangulation::printTriangle(int ind_tri, ofstream& stream_tri, ofstream& stream_circ, bool all_circ) {
    if (ind_tri < 0) { return; }
    triangles[ind_tri].print(stream_tri, stream_circ, all_circ);
}

// Print circle of the last triangle.
void Triangulation::printLastCircle(ofstream& stream_circle) {
    if (triangles.size() == 0) { return; }
    Triangle tmp = triangles[triangles.size() - 1];
    stream_circle << tmp.getCenter()[0] << " " << tmp.getCenter()[1] << " " << tmp.getRadius() << endl;
}

// Return number of triangles.
int Triangulation::size() {
    return nu_triangles;
}


///// Delaunay /////

// Algorithm "remove and build".
// Alse see book by Skvortsov
// https://indorsoft.ru/books/2002/SkvortsovAV-2002-01.Book(Trn).pdf
Delaunay::Delaunay(Field* p_fieldd, ofstream& logs_al)
    : logs_a(logs_al), p_field(p_fieldd) {
    triangulation = new Triangulation(p_field);
    writeLog("INIT");
}

// Main Delaunay triangulation algorithm.
Triangulation* Delaunay::mainAlgorithm() {
    writeLog("Begin mainAlgorithm");

    triangulation->addTriangle(findEnclosingTriangle());
    writeLog("\tFind enclosing triangle");

    for (int ind_poi = 0; ind_poi < p_field->numPoints(); ind_poi++) {
        set<int> polygon = triangulation->deleteTriangles(ind_poi);
        vector<int> sorted = sortPolygon(polygon, ind_poi);
        saveStep(ind_poi);
        createNewTriangles(sorted, ind_poi);
        saveStep(ind_poi);
    }
    writeLog("\tFind main triangulation");

    triangulation->deleteEnclosingTriangle();

    writeLog("\tDelete phantom triangles");
    writeLog("Find " + to_string(triangulation->size()) + " tringles");
    // saveStep(p_field->numPoints() - 1, true);
    saveStep(p_field->numPoints() - 1);

    string cur_template = (p_field->if_test) ? "test_" : "";
    ifstream anim_templ("data/templates/" + cur_template + "delaunay_animate.template");
    ofstream anim(p_field->output_directory + "/gnu_delaunay_animate.plt");

    string new_line;
    while (!anim_templ.eof()) {
        getline(anim_templ, new_line);
        anim << new_line << endl;
    }
    anim << "do for [i=1:" << to_string(step - 1) << "] {" << endl << "\tplot ";
    anim << "\t\t'" << p_field->output_directory << "/delaunay/edges_'.i.'.plt' with lines lw 2 lc rgb \"black\" title \"triangles\",\\" << endl;
    anim << "\t\t'" << p_field->output_directory << "/delaunay/circles_'.i.'.plt' with lines lw 3 lc rgb \"red\" title \"last triangle\",\\" << endl;
    anim << "\t\t'" << p_field->output_directory << "/delaunay/points_'.i.'.plt' index 0 w p lt 5 lc rgb \"blue\" title \"points\",\\" << endl;
    // anim << "\t\t'" << p_field->output_directory << "/delaunay/circles_'.i.'.plt' using 1:2:3 with circles lc rgb \"red\" title \"circles\",\\" << endl;
    anim << "\t\t'" << p_field->output_directory << "/delaunay/points_'.i.'.plt' index 1 w p lt 7 lc rgb \"red\" title \"current\"\n}" << endl;
    return triangulation;
}

// Find enclosing triangle.
Triangle Delaunay::findEnclosingTriangle() {
    writeLog("Begin findEnclosingTriangle");
    vector<double> box = p_field->getBox();
    double diff_x, diff_y;
    diff_x = box[1] - box[0];
    diff_y = box[3] - box[2];
    phantom_points.push_back(vector<double> {box[0] - (diff_x * 0.1), box[2] - (diff_y * 0.1)});
    phantom_points.push_back(vector<double> {box[0] - (diff_x * 0.01), box[3] + diff_y * 1.01});
    phantom_points.push_back(vector<double> {box[1] + (diff_x * 1.01), box[2] - (diff_y * 0.01)});
    writeLog("\tFind all phantom points");

    return Triangle{ phantom_points[0], phantom_points[1], phantom_points[2], p_field };
}

// Temporaly struct for sorting points by slant.
struct tmp_int_double
{
    int point;
    double slant;
};

// Sort points in the polygon so that they go "in a circle".
vector<int> Delaunay::sortPolygon(set<int> polygon, int ind_poi) {
    writeLog("Begin sortPolygone");
    vector<tmp_int_double> zip;
    for (int pol_poi : polygon) {
        double pol_x, pol_y, hipoth, angle;
        double poi_x = p_field->getPoint(ind_poi).getCoord()[0];
        double poi_y = p_field->getPoint(ind_poi).getCoord()[1];
        if (pol_poi >= 0) {
            pol_x = p_field->getPoint(pol_poi).getCoord()[0];
            pol_y = p_field->getPoint(pol_poi).getCoord()[1];
            hipoth = p_field->getDist(pol_poi, ind_poi);
        }
        else {
            pol_x = phantom_points[-pol_poi - 1][0];
            pol_y = phantom_points[-pol_poi - 1][1];
            hipoth = distPoints(p_field->getPoint(ind_poi), phantom_points[-pol_poi - 1]);
        }
        angle = acos((pol_x - poi_x) / hipoth);
        if (pol_y - poi_y < 0) {
            angle = 2 * PI - angle;
        }
        tmp_int_double tmp = { pol_poi, angle };
        zip.push_back(tmp);
    }
    sort(zip.begin(), zip.end(),
        [](tmp_int_double pol_1, tmp_int_double pol_2)
        {return pol_1.slant < pol_2.slant;});

    vector<int> sorted_points;
    for (tmp_int_double pol_point : zip) {
        sorted_points.push_back(pol_point.point);
    }
    return sorted_points;
}

// Create new triangles from vertexes of the polygon and the point.
void Delaunay::createNewTriangles(vector<int> sorted, int ind_poi) {
    writeLog("Begin createNewTriangle");
    vector<double> p_1, p_2;
    bool ph = false;
    for (int ind_vert = 0; ind_vert < sorted.size() - 1; ind_vert++) {
        ph = false;
        int ind_sorted = sorted[ind_vert];
        int next_sorted = sorted[ind_vert + 1];
        if (ind_sorted >= 0) {
            p_1 = p_field->getPoint(ind_sorted).getCoord();
        }
        else {
            ph = true;
            p_1 = phantom_points[(-ind_sorted) - 1];
        }
        if (next_sorted >= 0) {
            p_2 = p_field->getPoint(next_sorted).getCoord();
        }
        else {
            ph = true;
            p_2 = phantom_points[(-next_sorted) - 1];
        }

        triangulation->addTriangle(Triangle(p_1, p_2,
            vector<int> {ind_sorted, next_sorted, ind_poi}, ph, p_field));
        saveStep(ind_poi);
    }

    int first = sorted[0];
    int last = sorted[sorted.size() - 1];
    ph = false;
    if (first >= 0) {
        p_1 = p_field->getPoint(first).getCoord();
    } else {
        ph = true;
        p_1 = phantom_points[(-first) - 1];
    }
    if (last >= 0) {
        p_2 = p_field->getPoint(last).getCoord();
    } else {
        ph = true;
        p_2 = phantom_points[(-last) - 1];
    }
    triangulation->addTriangle(Triangle(p_1, p_2, vector<int> {first, last, ind_poi},
        ph, p_field));
    saveStep(ind_poi);
}

// Save one step of animation in data/delaunay
void Delaunay::saveStep(int cur_point, bool all_circ) {
    ofstream points_step(p_field->output_directory + "/delaunay/points_" + to_string(step) + ".plt");
    ofstream edges_step(p_field->output_directory + "/delaunay/edges_" + to_string(step) + ".plt");
    ofstream circles_step(p_field->output_directory + "/delaunay/circles_" + to_string(step) + ".plt");

    for (int ind_tri = 0; ind_tri < triangulation->size(); ind_tri++) {
        triangulation->printTriangle(ind_tri, edges_step, circles_step, all_circ);
    }

    // triangulation->printLastCircle(circles_step);
    edges_step << endl << endl;
    triangulation->printTriangle(triangulation->size() - 1, circles_step, circles_step, all_circ);

    for (int poi = 0; poi < cur_point; poi++) {
        p_field->getPoint(poi).print(points_step);
    }
    points_step << endl << endl;

    p_field->getPoint(cur_point).print(points_step);

    points_step.close();
    edges_step.close();
    step++;
}

// Write log-message with date-time note.
void Delaunay::writeLog(const string& message) {
    logs_a << timeLog() << "DELAUNAY (tringles "
        << triangulation->size() << "): " << message << endl;
}
