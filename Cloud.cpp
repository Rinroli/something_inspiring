// Copyright (c) 2020 Rinroli

#include "Cluster.h"

Cloud::Cloud(int cur_id_cloud, ofstream& logs_field, Field* field)
    : Cluster(cur_id_cloud, logs_field, field)
{
    p_field_points = &(field->points);
    p_field = field;
    id = cur_id_cloud;

    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cloud::Cloud(int idd, const vector<int>& points, ofstream& logs_field, Field* field)
    : Cluster(idd, points, logs_field, field)
{
    p_field_points = &(field->points);
    p_field = field;
    id = idd;

    writeLog("INIT >> id(" + to_string(id) + ")");
}

Cloud::~Cloud()
{
    id_points.clear();
    writeLog("DELETE");
}

// Write log-message with date-time note.
void Cloud::writeLog(const string& message) {
    logs_a << timeLog() << "CLOUD(id:" << id << "): " << message << endl;
}