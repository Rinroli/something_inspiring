// Copyright (c) 2020 Rinroli

#include "Field.h"

Field::Field(vector<bool> if_logs, vector<string> name_logs, stringstream& messagee) 
: message(messagee) {
    readonly = false;
    buffer = new Buffer(this);

    if (if_logs[0]) {
        logs.open("logs/" + name_logs[0], ios_base::app);
        logs << endl << "New session " << endl;
        writeLog("INIT");
    }
    if (if_logs[3]) {
        logs_a.open("logs/" + name_logs[3], ios_base::app);
        logs_a << endl << "New session " << timeLog() << endl;
    }
}

Field::~Field()
{
    clouds.clear();
    fclusters.clear();
    points.clear();
    logs.close();
    delete buffer;
    if (logs_a) { logs_a.close(); }
    writeLog("DELETE");
}

// Update size of field in all clusters
void Field::resizeAllClouds() {
    for (Cluster cl : clouds) {
        cl.resizeIndicator();
    }
}

// Create new normally distributed cloud of points.
bool Field::createCloud(double mX, double mY,
    double sX, double sY, int nu_points) {
    writeLog("CREATE CLOUD (normally distributed)");
    if (!readonly) {
        Cloud new_cloud(numClouds(), logs, this);
        for (int tmp = 0; tmp < nu_points; tmp++) {
            Point point(mX, mY, sX, sY, numPoints(), logs, new_cloud.id, nu_points);
            points.push_back(point);
            new_cloud += point.id;
        }
        clouds.push_back(new_cloud);
        resizeAllClouds();
        return true;
    }
    writeLog("Access denied");
    return false;
}

// Create new normally distributed cloud of points.
bool Field::createCloud(vector<Point> new_points) {
    writeLog("CREATE CLOUD (from buffer)");
    if (!readonly) {
        Cloud new_cloud(numClouds(), logs, this);
        for (Point point : new_points) {
            point.setID(numPoints());
            points.push_back(point);
            new_cloud += point.id;
        }
        clouds.push_back(new_cloud);
        resizeAllClouds();
        return true;
    }
    writeLog("Access denied");
    return false;
}

// Save clouds to the file.
void Field::print(ofstream& out_f)
{
    for (int ind_cl = 0; ind_cl < clouds.size(); ind_cl++) {
        Cloud cur_cl = clouds[ind_cl];
        // for (int ind_poi = 0; ind_poi < cur_cl.numPoints(); ind_poi++) {
        //     points[cur_cl[ind_poi].id].print(out_f);
        // }
        for (int id_poi : cur_cl.getPoints()) {
            cur_cl.printGnu(out_f);
        }
        out_f << endl << endl << endl;
    }
    writeLog("SAVE (clouds)");
}

// Show info about buffer.
void Field::showBuffer() {
    if (buffer->isEmpty()) {
        message << "Empty buffer->" << endl;
    }
    buffer->coutInfo(message);
    writeLog("SHOWBUFFER");
}

// Add cloud to the buffer->
bool Field::addToBuffer(int ind_cl) {
    if (numClouds() == 0) {
        message << "No clouds." << endl;
        return false;
    }
    if (ind_cl == -1) { (*buffer) += clouds[numClouds() - 1]; }
    else { (*buffer) += clouds[ind_cl]; }
    return true;
}

// Copy buffer to the field.
bool Field::putBuffer() {
    if (buffer->isEmpty()) {
        message << "Empty buffer->" << endl;
        return false;
    }
    buffer->putToField();
    return true;
}

// Rotate buffer->
bool Field::rotateBuffer(double alpha) {
    if (buffer->isEmpty()) {
        message << "Empty buffer->" << endl;
        return false;
    }
    buffer->rotatePoints(alpha);
    return true;
}

// Move buffer->
bool Field::moveBuffer(double x, double y) {
    if (buffer->isEmpty()) {
        message << "Empty buffer->" << endl;
        return false;
    }
    buffer->movePoints(x, y);
    return true;
}

// Zoom buffer->
bool Field::zoomBuffer(double k) {
    if (buffer->isEmpty()) {
        message << "Empty buffer->" << endl;
        return false;
    }
    buffer->zoomPoints(k);
    return true;
}

// Empty buffer->
bool Field::emptyBuffer() {
    if (buffer->isEmpty()) {
        message << "Buffer is already empty!" << endl;
        return false;
    }
    buffer->deletePoints();
    return true;
}

// Save clusters to the file.
void Field::print(int i, ofstream& out_f)
{
    FindClusters& cur_fd = getFCluster(i);
    for (int ind_cl = 0; ind_cl < cur_fd.numClusters(); ind_cl++) {
        Cluster cur_cl = cur_fd[ind_cl];
        vector<int> tmp = cur_cl.getPoints();
        for (int id_poi : cur_cl.getPoints()) {
            cur_cl.printGnu(out_f);
        }
        // for (int ind_poi = 0; ind_poi < cur_cl.numPoints(); ind_poi++) {
        //     int tmp = cur_cl.getIdPoint(ind_poi);
        //     (*(cur_cl.p_field_points))[tmp].print(out_f);
        // }
        out_f << endl << endl << endl;
    }
    writeLog("SAVE (clusters, FD #" + to_string(cur_fd.id) + ")");
}

// Getter for cluster by index.
FindClusters& Field::getFCluster(int i) {
    return fclusters[i];
}

// Getter for cluster by index.
vector<double> Field::getBox() {
    return containing_box;
}

// Return number of clouds.
int Field::numClouds() {
    return clouds.size();
}

// Return number of points.
int Field::numPoints() {
    return points.size();
}

// Return number of clusters.
int Field::numFClusters() {
    return fclusters.size();
}

// Return distance between two points (both by id)
double Field::getDist(int ind1, int ind2) {
    return matrix[ind1][ind2];
}

// Return distance between two points (by point and id)
double Field::getDist(Point point1, int ind2) {
    return matrix[point1.id][ind2];
}

// Calculate line through two points (by id).
// Equation ax + b = 0.
vector<double> Field::lineThroughPoints(int poi_1, int poi_2) {
    vector<double> p_1 = getPoint(poi_1).getCoord();
    vector<double> p_2 = getPoint(poi_2).getCoord();

    double a = (p_1[1] - p_2[1]) / (p_1[0] - p_2[0]);
    double b = p_1[1] - a * p_1[0];

    return vector<double> {a, b};
}

// Same as function for two points by id - but for vector<double> and id.
vector<double> Field::lineThroughPoints(vector<double> p_1, int poi_2) {
    vector<double> p_2 = getPoint(poi_2).getCoord();

    double a = (p_1[1] - p_2[1]) / (p_1[0] - p_2[0]);
    double b = p_1[1] - a * p_1[0];

    return vector<double> {a, b};
}

// Provide access to clouds by index.
Point& Field::operator[](int i) {
    if (i == -1) { return points[numPoints() - 1]; }
    return points[i];
}

// Say if modee is readonly.
bool Field::ifReadonly() {
    return readonly;
}

// Enter readonly 'Analysis' mode.
bool Field::enterAnalysis()
{
    message << "Entering 'Analysis' mode." << endl;
    if (numPoints() == 0) {
        message << "There is no points. You can do nothing with them." << endl;
        throw - 2;
    }
    try {
        readonly = true;
        resizeAllClouds();
        updateD();
        findBox();
    }
    catch (...) {
        writeLog("\tSomething went wrong.");
        return false;
    }
    writeLog("CHANGED MODE - ANALYSIS NOW");
    return true;
}

// Set value of the mark.
void Field::setAPoint(int i, int value) {
    points[i].a = value;
}

// Update distance matrix if field was changed.
void Field::updateD() {
    double dist;
    matrix.resize(numPoints());
    for (int i = 0; i < numPoints(); ++i) {
        matrix[i].resize(numPoints());
    }

    for (int first = 0; first < numPoints(); ++first) {
        for (int second = first; second < numPoints(); ++second) {
            dist = distPoints(points[first], points[second]);
            matrix[first][second] = dist;
            matrix[second][first] = dist;
        }
    }
    writeLog("MATRIX UPDATED");
}

// Find containing box and save it.
void Field::findBox() {
    vector<double> some_point = points[0].getCoord();

    containing_box[0] = some_point[0];
    containing_box[1] = some_point[0];
    containing_box[2] = some_point[1];
    containing_box[3] = some_point[1];

    for (int i = 1; i < numPoints(); ++i) {
        some_point = points[i].getCoord();
        if (some_point[0] > containing_box[1]) { containing_box[1] = some_point[0]; }
        if (some_point[0] < containing_box[0]) { containing_box[0] = some_point[0]; }
        if (some_point[1] > containing_box[3]) { containing_box[3] = some_point[1]; }
        if (some_point[1] < containing_box[2]) { containing_box[2] = some_point[1]; }
    }
    writeLog("BOX UPDATED");
}

// Add new FindClusters to the vector with them.
void Field::addFC(FindClusters new_fc) {
    new_fc.setID(fclusters.size());
    fclusters.push_back(new_fc);
    writeLog("ADD FIND_CLUSTERS - Source " + new_fc.source);
}

// Get binary matrix by index.
// >> -1 is the last one.
BinMatrix& Field::getBinMatrix(int i) {
    if (i == -1) { return bin_matrixes[bin_matrixes.size() - 1]; }
    return bin_matrixes[i];
}

// Getter for points by index.
Point& Field::getPoint(int i) {
    return points[i];
}

// Getter for clouds by index.
Cloud& Field::getCloud(int i) {
    return clouds[i];
}

// Return number of binary matrixes.
int Field::numBinMatrix() {
    return bin_matrixes.size();
}

// Create matrix (graph) with info about incident points.
void Field::binMatrix(double delta) {
    writeLog("Begin creating binary matrix (" + to_string(delta) + ")");
    BinMatrix matrix_inc(numPoints(), delta);
    for (int ind_poi = 0; ind_poi < numPoints(); ++ind_poi) {
        matrix_inc[ind_poi][ind_poi] = 0;
        for (int ind_poi_sec = ind_poi; ind_poi_sec < numPoints(); ++ind_poi_sec) {
            if ((ind_poi != ind_poi_sec) & (matrix[ind_poi][ind_poi_sec] < delta)) {
                matrix_inc[ind_poi][ind_poi_sec] = 1;
                matrix_inc[ind_poi_sec][ind_poi] = 1;
            }
            else {
                matrix_inc[ind_poi][ind_poi_sec] = 0;
                matrix_inc[ind_poi_sec][ind_poi] = 0;
            }
        }
    }
    bin_matrixes.push_back(matrix_inc);
    writeLog("\tCREATE binary matrix");
}

// Create matrix (graph) with info about incident points and neighbors.
void Field::binDBMatrix(double delta, int k) {
    writeLog("Begin creating binary db matrix (" + to_string(delta) + ", " + to_string(k) + ")");
    BinMatrix matrix_inc(numPoints(), delta, k);
    for (int ind_poi = 0; ind_poi < numPoints(); ++ind_poi) {
        int tmp = 0;
        matrix_inc[ind_poi][ind_poi] = 0;
        for (int ind_poi_sec = ind_poi; ind_poi_sec < numPoints(); ++ind_poi_sec) {
            if ((ind_poi != ind_poi_sec) & (matrix[ind_poi][ind_poi_sec] < delta)) {
                matrix_inc[ind_poi][ind_poi_sec] = 1;
                matrix_inc[ind_poi_sec][ind_poi] = 1;
                tmp++;
            }
            else {
                matrix_inc[ind_poi][ind_poi_sec] = 0;
                matrix_inc[ind_poi_sec][ind_poi] = 0;
            }
        }
        if (tmp < k) {
            matrix_inc.marks[ind_poi] = -1;
            points[ind_poi].mark = "dust";
        }
    }

    for (int ind_poi = 0; ind_poi < numPoints(); ++ind_poi) {
        if (matrix_inc.marks[ind_poi] == -1) {
            for (int ind_poi_sec = 0; ind_poi_sec < numPoints(); ++ind_poi_sec) {
                if ((matrix_inc.marks[ind_poi_sec] == 0) & (matrix_inc[ind_poi][ind_poi_sec])) {
                    matrix_inc.marks[ind_poi] = -2;
                    break;
                }
            }
        }
    }
    for (int j = 0; j < numPoints(); ++j) {
        if (matrix_inc.marks[j] == -2) {
            points[j].mark = "peripheral";
            matrix_inc.marks[j] = 0;
        }
    }
    bin_matrixes.push_back(matrix_inc);
    writeLog("\tCREATE binary matrix with neighbors");
}

// Begin test.
void Field::beginTest(const string& output_dir, const string& output_na) {
    if_test = true;
    output_name = output_na;
    output_directory = output_dir;
}

// Print edges to the file graph_edges.plt.
void Field::drawBinGraph(int i) {
    ofstream graph_edges(output_directory + "/" + output_name + "_edges.plt");
    ofstream graph_points(output_directory + "/" + output_name + "_points.plt");

    BinMatrix& matrix_inc = getBinMatrix(i);
    for (int poi = 0; poi < numPoints(); ++poi) {
        if (points[poi].mark == "dust") {
            points[poi].print(graph_points);
            continue;
        }
        for (int nei_p = 0; nei_p < numPoints(); nei_p++) {
            if (((points[poi].mark == "peripheral") | (poi < nei_p)) &
                (matrix_inc[poi][nei_p]) & (points[nei_p].mark == "base")) {
                points[poi].print(graph_edges);
                points[nei_p].print(graph_edges);
                graph_edges << endl << endl;
            }
        }
    }
    graph_edges.close();

    graph_points << endl << endl;
    for (Point point : points) {
        if (point.mark == "peripheral") {
            point.print(graph_points);
        }
    }
    graph_points << endl << endl;
    for (Point point : points) {
        if (point.mark == "base") {
            point.print(graph_points);
        }
    }
    graph_points.close();
}

// Create minimal spanning tree.
void Field::minSpanTree() {
    writeLog("Begin minSpanTree");
    p_tree = new Tree(points[0], logs_a);

    vector<bool> already_taken(numPoints());
    already_taken[0] = true;
    int min_ind, min_taken_ind;

    for (int step = 0; step < numPoints() - 1; step++) {
        double min_dist = INF;
        for (int ind = 0; ind < numPoints(); ind++) {
            if (!already_taken[ind]) {
                for (int taken_poi = 0; taken_poi < numPoints(); taken_poi++) {
                    if (already_taken[taken_poi]) {
                        if (getDist(ind, taken_poi) < min_dist) {
                            min_dist = getDist(ind, taken_poi);
                            min_ind = ind; min_taken_ind = taken_poi;
                        }
                    }
                }
            }
        }
        if (min_taken_ind == 0) { p_tree->addVert(points[min_ind], min_dist); }
        else { (*(p_tree->findIndex(min_taken_ind))).addVert(points[min_ind], min_dist); }
        already_taken[min_ind] = true;
    }

    // ofstream graph("data/tree_data.plt");
    ofstream graph(output_directory + "/" + output_name + "_data.plt");
    p_tree->displayTree(graph);
}

// Write log-message with date-time note.
void Field::writeLog(const string& message)
{
    logs << timeLog() << "FIELD: " << message << endl;
}
