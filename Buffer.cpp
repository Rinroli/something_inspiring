// Copyright (c) 2020 Rinroli

#include "Buffer.h"

Buffer::Buffer(Field* p_fieldd)
    : logs_f(p_fieldd->logs), p_field(p_fieldd)
{
    writeLog("INIT");
}

Buffer::~Buffer()
{
    points.clear();
    writeLog("DELETE");
}

// Addition of id points from cluster to buffer.
void Buffer::addCluster(Cluster& new_cluster) {
    for (int id_in_cl = 0; id_in_cl < new_cluster.id_points.size(); id_in_cl++) {
        Point new_point(new_cluster[id_in_cl]);
        new_point.setID(id_in_cl);
        points.push_back(new_point);
    }
    writeLog("ADD_POINTS_FROM_CLOUD #" + to_string(new_cluster.id));
}

// Simple addition of new cluster to buffer.
void operator+=(Buffer& left, Cluster& new_cluster) {
    left.addCluster(new_cluster);
}

// Rotate points relative to the center by an alpha angle
void Buffer::rotatePoints(double alpha) {
    for (int ind_point = 0; ind_point < points.size(); ind_point++) {
        points[ind_point].rotatePoint(alpha);
    }
    writeLog("ROTATE (" + to_string(alpha) + ")");
}

// Move points of the buffer.
void Buffer::movePoints(double x, double y) {
    for (int ind_point = 0; ind_point < points.size(); ind_point++) {
        points[ind_point].movePoint(x, y);
    }
    writeLog("MOVE (" + to_string(x) + ", " + to_string(y) + ")");
}

// Zoom in or zoom out k times (relative to the center) points from the buffer.
void Buffer::zoomPoints(double k) {
    for (int ind_point = 0; ind_point < points.size(); ind_point++) {
        points[ind_point].zoomPoint(k);
    }
    writeLog("ZOOM (" + to_string(k) + ")");
}

// Delete all points from the buffer.
void Buffer::deletePoints() {
    points.clear();
}

// Check if buffer is empty.
bool Buffer::isEmpty() {
    if (points.size() == 0) { return true; }
    return false;
}

// Print some info about buffer to standart output.
void Buffer::coutInfo() {
    cout << "Buffer:" << endl;
    for (Point point : points) {
        cout << "\t" << point << endl;
    }
}

// Copy all points from buffer to field as one cloud.
void Buffer::putToField() {
    p_field->createCloud(points);
    writeLog("PUT_TO_FIELD");
}

// Write log-message with date-time note.
void Buffer::writeLog(const string& message) {
    logs_f << timeLog() << "BUFFER: " << message << endl;
}