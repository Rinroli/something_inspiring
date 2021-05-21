// Copyright (c) 2020 Rinroli

#include "Prediction.h"

Prediction::Prediction(vector<double> new_pointt, Triangulation* p_triangulation,
    Field* p_field,  ofstream& logs_aa) 
: logs_a(logs_aa), new_point(new_pointt) {
    int predict_ind = p_triangulation->findTriangle(new_point);
    Triangle* cur_triangle = (*p_triangulation)[predict_ind];

    vector<int> tri_ind = cur_triangle->getVerts();
    for (int ind_vert : tri_ind) {
        triangle_verts.push_back((p_field->getPoint(ind_vert)).getCoord());
    }

    for (int i = 0; i < 3; i++) {
        vert_val.push_back(funcValue(triangle_verts[i]));
    }
    writeLog("End init");
}

// Predict value of the given point
double Prediction::predictPoint() {
    vector<double> plane = findPlane();

    return (-plane[3] - plane[0] * new_point[0] - plane[1] * new_point[1]) / plane[2];
}

// Find plan by three given points of the triangle
vector<double> Prediction::findPlane() {
    float a1 = triangle_verts[1][0] - triangle_verts[0][0];
    float b1 = triangle_verts[1][1] - triangle_verts[0][1];
    float c1 = vert_val[1] - vert_val[0];
    float a2 = triangle_verts[2][0] - triangle_verts[0][0];
    float b2 = triangle_verts[2][1] - triangle_verts[0][1];
    float c2 = vert_val[2] - vert_val[0];
    double a = b1 * c2 - b2 * c1;
    double b = a2 * c1 - a1 * c2;
    double c = a1 * b2 - b1 * a2;
    double d = (-a * triangle_verts[0][0] - b * triangle_verts[0][1] - c * vert_val[0]);

    return vector<double> {a,b,c,d};
}

// Logging
void Prediction::writeLog(const string& message) {
    logs_a << timeLog() << "PREDICT: " << message << endl;
}