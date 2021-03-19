// Copyright (c) 2020 Rinroli

#include "Forel.h"

Forel::Forel(double RR, const vector<Point>& real_pointss, Field* p_fieldd,
    ofstream& logs_al, int gl_stepp = 0, int framee = 0, int max_clusterss = 0)
    : real_points(real_pointss), logs_a(logs_al), R(RR), global_step(gl_stepp), frame(framee),
    p_field(p_fieldd), max_clusters(max_clusterss)
{
    f_clusters = new FindClusters(logs_al, "FOREL, step #" + to_string(2 - gl_stepp));
    writeLog("INIT");
    for (int ind = 0; ind < real_points.size(); ind++) {
        points.push_back(ind);
    }
}

Forel::~Forel() {
    points.clear();
    centroids.clear();
}

// Call algorithm.
vector <FindClusters> Forel::mainAlgorithm() {
    writeLog("Start mainAlgorithml");

    while (points.size() != 0) {
        if (step > 500) {
            cout << "To many clusters!" << endl;
            break;
        }
        (*f_clusters) += oneNewCluster();
        newFrame();
        writeLog("New cluster #" + to_string(step) + " created");
    }

    cout << "Find " << f_clusters->numClusters() << " clusters" << endl;

    if (global_step == 2) {
        saveAnimation();
        exported_points.push_back((*f_clusters));
        return vector<FindClusters> {(*f_clusters)};
    }

    if (step > max_clusters) { max_clusters = step; }
    centroid_clustering = new Forel(R * 3, centroids, p_field,
        logs_a, global_step + 1, frame, max_clusters);
    exported_points = (*centroid_clustering).mainAlgorithm();

    exportPoints();

    return exported_points;
}

// Export points to field-points.
// Remember what points was in the neighbourhoods.
void Forel::exportPoints() {
    if (global_step == 2) {
        return;
    }
    vector<FindClusters> new_exp_poi;
    for (int ind_exp = 0; ind_exp < exported_points.size(); ind_exp++) {
        FindClusters new_fc(logs_a, "FOREL, exported, level #" + to_string(ind_exp));
        for (Cluster old_clu : (exported_points[ind_exp]).clusters) {
            Cluster new_clu(step, logs_a, p_field, &real_points);
            for (int ind_point : old_clu.id_points) {
                for (int new_point_ind : (*f_clusters)[ind_point].id_points) {
                    new_clu += new_point_ind;
                }
            }
            new_fc += new_clu;
        }
        new_exp_poi.push_back(new_fc);
    }
    exported_points.swap(new_exp_poi);
    exported_points.push_back((*f_clusters));
}


// Getter for point by index in forel-points.
Point& Forel::getPoint(int i) {
    return real_points[points[i]];
}

// Remove point from unclustered by id.
void Forel::removePoint(int i) {
    for (int ind_in_unclust = 0; ind_in_unclust < points.size(); ind_in_unclust++) {
        if (i == points[ind_in_unclust]) {
            points.erase(points.begin() + ind_in_unclust);
            return;
        }
    }
}

// Find new Cluster and delete it from list.
Cluster Forel::oneNewCluster() {
    int new_center_id = randInt(points.size());
    vector<double> center = getPoint(new_center_id).getCoord();
    centroids.push_back(Point(center, step, -1, logs_a));
    newFrame();
    Cluster neigh_cluster = findNeighbourhood(center);
    vector<double> new_center = neigh_cluster.findAverage();
    centroids.pop_back();
    centroids.push_back(Point(new_center, step, -1, logs_a));
    newFrame();
    int tmp_counter = 0;
    while (distPoints(center, new_center) > EPS) {
        if (tmp_counter > 100) {
            cout << "unstable" << endl;
            break;
        }
        center = new_center;
        Cluster tmp(neigh_cluster);
        neigh_cluster = findNeighbourhood(center);
        new_center = neigh_cluster.findAverage();
        centroids.pop_back();
        centroids.push_back(Point(new_center, step, -1, logs_a));
        newFrame();
        tmp_counter++;
    }
    for (int ind = 0; ind < neigh_cluster.numPoints(); ind++) {
        removePoint(neigh_cluster[ind].id);
    }
    step++;
    return neigh_cluster;
}

// Find all points in the R-neighbourhood of the point
Cluster Forel::findNeighbourhood(const vector<double>& center) {
    Cluster res(step, logs_a, p_field, &real_points);
    for (int other_point : points) {
        if (distPoints(real_points[other_point], center) < R) {
            res += other_point;
        }
    }
    return res;
}

// Save command file for animation for gnuplot.
void Forel::saveAnimation() {
    string cur_template = (p_field->if_test) ? "test_" : "";
    ifstream anim_templ("data/templates/" + cur_template + "forel_animate.template");
    ofstream anim(p_field->output_directory + "/gnu_forel_animate.plt");

    string new_line;
    while (!anim_templ.eof()) {
        getline(anim_templ, new_line);
        anim << new_line << endl;
    }
    anim << "do for [i=0:" << to_string(frame - 1) << "] {" << endl << "\tplot ";
    for (int ind_k = 0; ind_k < max_clusters; ind_k++) {
        anim << "\t\t'" << p_field->output_directory << "/forel/clusters_'.i.'.plt' index " <<
            to_string(ind_k) << " w p title \"#" << to_string(ind_k) << "\",\\" << endl;
    }
    anim << "\t\t'" << p_field->output_directory << "/forel/circles_'.i.'.plt' using 1:2:3"
        << "with circles lc rgb \"red\" title \"circles\"\n}";
}

// Create new frame 'forel/clusters_i.plt' and 'forel/circles_i.plt'
void Forel::newFrame() {
    writeLog("New frame");
    ofstream file_clu(p_field->output_directory + "/forel/clusters_" + to_string(frame) + ".plt");
    ofstream file_circ(p_field->output_directory + "/forel/circles_" + to_string(frame) + ".plt");

    for (int unclust_p : points) {
        Point tmp = real_points[unclust_p];
        file_clu << tmp.x << " " << tmp.y << endl;
    }

    file_clu << endl << endl;

    for (int nu_clu = 0; nu_clu < f_clusters->numClusters(); nu_clu++) {
        (*f_clusters)[nu_clu].printGnu(file_clu);

        file_clu << endl << endl;
    }

    for (Point cent : centroids) {
        file_circ << cent.x << " " << cent.y << " " << R << endl;
    }

    file_circ.close();
    file_clu.close();
    frame++;
}

// Write log-message with date-time note.
void Forel::writeLog(const string& message) {
    logs_a << timeLog() << "FOREL #" << global_step << " : " << message << endl;
}
