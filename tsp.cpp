// #pragma GCC optimize("Ofast")
// #pragma GCC optimize ("unroll-loops")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#include <bits/stdc++.h>
#include "tsp_solver.h"

using namespace std;

const double PI = 3.141592;

string file_name, junk;
string test_name, weight_type;
vector <pair<double, double>> coords;
vector<vector<long long>> distances;
int dimension;

void calc_max_distances() {
    for (int i = 0; i < dimension; i++) {
        vector <long long> dists;
        for (int j = 0; j < i; j++)
            dists.push_back(distances[j][i]);
        dists.push_back(0);
        for (int j = i + 1; j < dimension; j++) {
            double dx = abs(coords[i].first - coords[j].first);
            double dy = abs(coords[i].second - coords[j].second);

            dists.push_back((long long) (max(dx, dy) + 0.5));
        }
        distances.push_back(dists);
    }
}

void calc_manhattan_distances() {
    for (int i = 0; i < dimension; i++) {
        vector <long long> dists;
        for (int j = 0; j < i; j++)
            dists.push_back(distances[j][i]);
        dists.push_back(0);
        for (int j = i + 1; j < dimension; j++) {
            double dx = abs(coords[i].first - coords[j].first);
            double dy = abs(coords[i].second - coords[j].second);

            dists.push_back((long long) (dx + dy + 0.5));
        }
        distances.push_back(dists);
    }
}

void calc_euc_distances(int type) {
    /*
    type =
    0 : EUC_2D
    1 : CEIL_2D
    2 : ATT
    */
    for (int i = 0; i < dimension; i++) {
        vector <long long> dists;
        for (int j = 0; j < i; j++)
            dists.push_back(distances[j][i]);
        dists.push_back(0);
        for (int j = i + 1; j < dimension; j++) {
            double dist = (coords[i].first - coords[j].first) * (coords[i].first - coords[j].first)
                            + (coords[i].second - coords[j].second) * (coords[i].second - coords[j].second);
            if (type == 1)
                dists.push_back(ceil(sqrt(dist)));
            else if (type == 0)
                dists.push_back((long long)(sqrt(dist) + 0.5));
            else if (type == 2) {
                dist = sqrt(dist / 10.0);
                long long dist2 = (long long) (dist + 0.5);
                if (dist2 < dist)
                    dist2++;
                dists.push_back(dist2);
            }
        }
        distances.push_back(dists);
    }
}

void calc_geo_distances(double radius = 6378.388) {
    for (int i = 0; i < dimension; i++) {
        long long deg = coords[i].first + 0.5;
        double mn = coords[i].first - deg;
        double lat1 = PI * (deg + 5.0 * mn / 3.0) / 180.0;
        deg = coords[i].second + 0.5;
        mn = coords[i].second - deg;
        double long1 = PI * (deg + 5.0 * mn / 3.0) / 180.0;

        vector <long long> dists;
        for (int j = 0; j < i; j++)
            dists.push_back(distances[j][i]);
        dists.push_back(0);
        for (int j = i + 1; j < dimension; j++) {
            deg = coords[j].first + 0.5;
            mn = coords[j].first - deg;
            double lat2 = PI * (deg + 5.0 * mn / 3.0) / 180.0;
            deg = coords[j].second + 0.5;
            mn = coords[j].second - deg;
            double long2 = PI * (deg + 5.0 * mn / 3.0) / 180.0;

            double a = cos(long1 - long2);
            double b = cos(lat1 - lat2);
            double c = cos(lat1 + lat2);
            double dist = radius * acos(0.5 * ((1.0 + a) * b - (1.0 - a) * c)) + 1.0;
            dists.push_back((long long)(dist + 0.5));
        }
        distances.push_back(dists);
    }
}

void save(vector<pair<int, int>>& tour) {
    cout << "saving the tour" << endl;
    ofstream output_file("sol_" + file_name);
    long long w = 0;
    for (int i = 0; i < dimension; i++)
        w += distances[i][tour[i].first] + distances[i][tour[i].second];
    w /= 2;
    cout << "weight: " << w << endl;
    output_file << w << '\n';
    int cur = 0;
    for (int i = 0; i < dimension; i++) {
        output_file << cur << '\n';
        cur = tour[cur].second;
    }
}

void read_file(string file_name) {
    cout << "reading the file..." << endl;
    ifstream input_file(file_name);
    input_file >> junk;
    input_file >> test_name;
    getline(input_file, junk);
    getline(input_file, junk);
    getline(input_file, junk);
    input_file >> junk;
    input_file >> dimension;
    input_file >> junk;
    input_file >> weight_type;
    input_file >> junk;
    if (junk != "NODE_COORD_SECTION") {
        getline(input_file, junk);
        getline(input_file, junk);
    }
    if (junk == "NODE_COORD_SECTION") {
        coords.resize(dimension);
        for (int i = 0; i < dimension; i++) {
            int index;
            double x, y;
            input_file >> index >> x >> y;
            coords[index - 1] = {x, y};
        }
    }

    if (weight_type == "EUC_2D")
        calc_euc_distances(0);
    else if (weight_type == "CEIL_2D")
        calc_euc_distances(1);
    else if (weight_type == "ATT")
        calc_euc_distances(2);
    else if (weight_type == "GEO")
        calc_geo_distances();
    else if (weight_type == "MAX_2D")
        calc_max_distances();
    else if (weight_type == "MAN_2D")
        calc_manhattan_distances();
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    cin >> file_name;
    long long t = time(NULL);
    read_file(file_name);
	vector<pair<int, int>> tour = solve(distances, true, false);
    save(tour);
    long long tt = time(NULL);
    cout << tt - t << endl;
    return 0;
}
