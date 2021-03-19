// Copyright (c) 2020 Rinroli

#include "Triangle.h"

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