// Copyright (c) 2020 Rinroli

#include <cmath>

#include "my_functions.h"
#include "Point.h"

using namespace std;

// Normal distribution.
double normalPoint(double mu, double sigma, int nu_points) {
    double sum = 0;
    for (int i = 0; i < nu_points; ++i) {
        sum += getRandom();
    }
    return mu + (sum / nu_points) * sigma;
}

// Return random double in [-1, 1].
double getRandom() {
    int po = rand() % 10000 - 5000;
    double u_1 = (static_cast<double>(po) / 10000);
    return u_1;
}


// Return random integer from [0, max_val)
int randInt(int max_val) {
    return rand() % max_val;
}

// Return clear string with time info for logs.
string timeLog() {
    time_t seconds = time(0);
    tm* timeinfo = localtime(&seconds);
    string tmp = to_string(timeinfo->tm_mday);
    tmp += "." + to_string(timeinfo->tm_mon + 1) + " -- " + to_string(timeinfo->tm_hour);
    tmp += ":" + to_string(timeinfo->tm_min) + " -> ";
    return tmp;
}

// Return distance between two points.
double distPoints(Point f_poi, Point s_poi) {
    return sqrt((f_poi.x - s_poi.x) * (f_poi.x - s_poi.x) +
        ((f_poi.y - s_poi.y) * (f_poi.y - s_poi.y)));
}

// Return distance between point and vector with coords.
double distPoints(Point point, const vector<double>& center) {
    return sqrt((point.x - center[0]) * (point.x - center[0]) +
        ((point.y - center[1]) * (point.y - center[1])));
}

// Return distance between two vectors with coords.
double distPoints(const vector<double>& center1, const vector<double>& center2) {
    return sqrt((center1[0] - center2[0]) * (center1[0] - center2[0]) +
        ((center1[1] - center2[1]) * (center1[1] - center2[1])));
}

// Return distance between vectors.
double distVectors(const vector<double>& vector_1, const vector<double>& vector_2) {
    if (vector_1.size() != vector_2.size()) { cout << "!!!" << endl;return 0; }
    double tmp_sum = 0;
    for (int i = 0; i < vector_1.size(); i++) {
        tmp_sum += (vector_1[i] - vector_2[i]) * (vector_1[i] - vector_2[i]);
    }
    return sqrt(tmp_sum);
}

// Return determinant of 3D matrix.
double findDeterminant3(vector<vector<double>> matrix) {
    double result = 0;
    result += matrix[0][0] * matrix[1][1] * matrix[2][2];
    result += matrix[0][1] * matrix[1][2] * matrix[2][0];
    result += matrix[0][2] * matrix[1][0] * matrix[2][1];
    result -= matrix[0][2] * matrix[1][1] * matrix[2][0];
    result -= matrix[0][1] * matrix[1][0] * matrix[2][2];
    result -= matrix[0][0] * matrix[1][2] * matrix[2][1];
    return result;
}

// Return determinant for 2d matrix.
double determinant(const vector<vector<double>>& sigma) {
    return sigma[0][0] * sigma[1][1] - sigma[0][1] * sigma[1][0];
}

// Return vector with eigenvectors and eigenvalues for 2d matrix
//  as {eivector1, eivector2, {eivalue1, eivalue2}}
//  empty vector if they don't exist.
vector<vector<double>> eigen(const vector<vector<double>>& sigma) {
    double a = sigma[0][0], c = sigma[1][0];
    vector<double> eigenvalues = findEigenvalues(sigma);
    vector<vector<double>> result;
    if (eigenvalues.size() == 0) { return result; }

    result.push_back(vector<double>{a - eigenvalues[0], c});
    result.push_back(vector<double>{a - eigenvalues[1], c});
    result.push_back(eigenvalues);
    return result;
}

// Return two or zero eigenvalues.
vector<double> findEigenvalues(const vector<vector<double>>& sigma) {
    double a = sigma[0][0], d = sigma[1][1];
    double descr = (a + d) * (a + d) - 4 * determinant(sigma);
    vector<double> lambdas;
    if (descr >= 0) {
        lambdas.push_back(((a + d) + sqrt(descr)) / 2.);
        lambdas.push_back(((a + d) - sqrt(descr)) / 2.);
    }
    return lambdas;
}

// Return true if given string is an number.
bool isNumber(string test_str) {
    for (char ch : test_str) {
        if (not isdigit(ch)) { return false; }
    }
    return true;
}

// Return colored string. Wow!
string colorString(string given, string color, bool bold) {
    string color_prefix = "\e[";

    if (bold) { color_prefix += "1;"; }

    if (color == "red") {
        color_prefix += "31";
    } else if (color == "green") {
        color_prefix += "32";
    } else if (color == "blue") {
        color_prefix += "34";
    } else if (color == "cyan") {
        color_prefix += "36";
    } else if (color == "grey") {
        color_prefix += "37";
    }

    return color_prefix + "m" + given + "\e[0m";
}

// Return function value of the point
double funcValue(vector<double> point) {
    // return point[0] * point[0] + point[1] * point[1];

    return cos(3*(point[0] + point[1]));
}