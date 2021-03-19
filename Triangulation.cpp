// Copyright (c) 2020 Rinroli

#include "Triangulation.h"

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