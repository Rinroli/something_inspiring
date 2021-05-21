// Copyright (c) 2020 Rinroli

#include "Point.h"

using namespace std;

Point::Point(double mX, double mY, double sX, double sY,
    int idd, ofstream& logs_field, int id_cloudd, int nu_points)
    : logs_f(logs_field)
{
    id = idd;
    x = normalPoint(mX, sX, nu_points);
    y = normalPoint(mY, sY, nu_points);
    id_cloud = id_cloudd;
}

Point::Point(const vector<double>& coords, int idd, int id_cloudd, ofstream& logs)
    : x(coords[0]), y(coords[1]), id(idd), id_cloud(id_cloudd), logs_f(logs)
{
}

Point::Point(ofstream& logss)
    : x(0), y(0), id(-1), id_cloud(-1), logs_f(logss)
{
}

Point::Point(double xx, double yy, ofstream& logss)
    : x(xx), y(yy), id(-1), id_cloud(-1), logs_f(logss)
{
}

// Changes point to the given one.
void Point::changeTo(Point point) {
    x = point.x;
    y = point.y;
    id = point.id;
    id_cloud = point.id_cloud;
    a = point.a;
    b = point.b;
    mark = point.mark;
}

// Set new id.
void Point::setID(int idd) {
    id = idd;
}

// Set new coords.
void Point::setCoords(double new_x, double new_y) {
    x = new_x; y = new_y;
}

// Return coords of the point.
vector<double> Point::getCoord() {
    vector<double> coords = { x, y };
    return coords;
}

// Return X-coord of the point.
double Point::getX() {
    return x;
}

// Return Y-coord of the point.
double Point::getY() {
    return y;
}

// Rotate point relative to the center on alpha angle in grad.
void Point::rotatePoint(double alpha) {
    alpha = alpha * PI / 180;
    double new_x = x * cos(alpha) - y * sin(alpha);
    double new_y = x * sin(alpha) + y * cos(alpha);
    x = new_x; y = new_y;
}

// Move point on vector (new_x, new_y).
void Point::movePoint(double new_x, double new_y) {
    x += new_x;
    y += new_y;
}

// Zoom in or zoom out k times (relative to the center) point
void Point::zoomPoint(double k) {
    x *= k;
    y *= k;
}

// Save coords of the point to the file.
void Point::print(ofstream& out_f) {
    out_f << x << "  " << y << " ---- " << id_cloud << endl;
}

// Put point to the given stream.
ostream& operator<<(ostream& out, const Point& point) {
    out << "(" << point.x << ", " << point.y << ")";
    return out;
}

// Write log-message with date-time note.
void Point::writeLog(const string& message) {
    logs_f << timeLog() << "POINT(id:" << id << "): " << message << endl;
}
