// Copyright (c) 2020 Rinroli

#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include "Point.h"

class Point;

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

double distPoints(Point f_poi, Point s_poi);
double distPoints(Point point, const vector<double>& center);
double distPoints(const vector<double>& center1, const vector<double>& center2);

bool isNumber(string test_str);

string colorString(string given_str, string color="red", bool bold=true);

double funcValue(vector<double> point);