// Copyright (c) 2020 Rinroli

#include "Triangulation.h"

// Delete triangles that don't satisfy the Delaunay condition.
// Return vector of points of these triangles.
set<int> Triangulation::deleteTriangles(int ind_poi) {
    bool next = false;
    set<int> polygon_unsorted;
    for (int ind_tri = 0; ind_tri < nu_triangles; ind_tri++) {
        next = false;
        vector<int> verts = triangles[ind_tri].getVerts();
        for (int id_vert : verts) {
            if (id_vert == ind_poi) {
                next = true;
                for (int i = 0; i < 3; i++) {
                    if (verts[i] != ind_poi) {
                        polygon_unsorted.insert(verts[i]);
                    }
                }
                triangles.erase(triangles.begin() + ind_tri);
                ind_tri--;
                nu_triangles--;
                break;
            }
        }
        if ((not next) and checkDelaunayCondition(triangles[ind_tri], ind_poi)) {
            for (int i = 0; i < 3; i++) {
                if (verts[i] != ind_poi) {
                    polygon_unsorted.insert(verts[i]);
                }
            }

            triangles.erase(triangles.begin() + ind_tri);
            ind_tri--;
            nu_triangles--;
        }
    }
    return polygon_unsorted;
}


// Copy Triangulation
Triangulation::Triangulation(Triangulation* tr_to_copy) 
: p_field(tr_to_copy->p_field), nu_triangles(tr_to_copy-> nu_triangles),
triangles(tr_to_copy->triangles)
{
}

// Check Delaunay conditions for triangle and point.
bool Triangulation::checkDelaunayCondition(Triangle& tri, int ind_poi) {
    return distPoints((p_field)->getPoint(ind_poi), tri.getCenter()) < tri.getRadius();
}

// Search for a triangle containing a given point
int Triangulation::findTriangle(vector<double> new_point) {
    for (int ind_tri = 0; ind_tri < nu_triangles; ind_tri++) {
        if (triangles[ind_tri].containPoint(new_point)) { return ind_tri; }
    }
    return -1;
}

// Return triangle by index
Triangle* Triangulation::operator[](int i) {
    return &(triangles[i]);
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