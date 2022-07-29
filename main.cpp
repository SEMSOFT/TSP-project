// #pragma GCC optimize("Ofast")
// #pragma GCC optimize ("unroll-loops")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

vector <vector<double>> distances;
int dimension;
string file_name, junk;
string test_name, weight_type;
vector<int> pi;
vector <pair<double, double>> coords;
vector<vector<int>> nearest;

void calc_euc_distances() {
    for (int i = 0; i < dimension; i++) {
        vector <double> dists;
        for (int j = 0; j < i; j++)
            dists.push_back(distances[j][i]);
        dists.push_back(0);
        for (int j = i + 1; j < dimension; j++) {
            double dist = (coords[i].first - coords[j].first) * (coords[i].first - coords[j].first)
                            + (coords[i].second - coords[j].second) * (coords[i].second - coords[j].second);
            dists.push_back(sqrt(dist));
        }
    }
}

void read_file(string file_name) {
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
        calc_euc_distances();

}

bool chooseX(vector<pair<int, int>> &tour, int fs, int last, double gain, set<pair<int, int>> X, set<pair<int, int>> Y){}

bool improve(vector<pair<int, int>> &tour){
    int n = tour.size();
    for(int t1 = 0; t1 < n; t1++){
        int tmp[2] = {tour[t1].first, tour[t1].second};
        for(int i = 0; i < 2; i++){
            int t2 = tmp[i];
            set<pair<int, int>> X;
            X.insert({t1, t2});

            for(int t3: nearest[t2]){
                if(t3 == tour[t2].first || t3 == tour[t2].second)
                    continue;
                set<pair<int, int>> Y;
                Y.insert({t2, t3});
                double gain = distances[t1][t2] - distances[t2][t3];

                if(gain > 0){
                    if chooseX(tour, t1, t3, gain, X, Y){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

vector<pair<int, int>> init() {
    pi = edge_transform(distances);
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (i != j)
                distances[i][j] += pi[i] + pi[j];
        }
    }
    get_a_nearness(distances, 1);
    return get_farthest_insertion_tour(distances);
}

vector<pair<int, int>> solve(){
    bool improved = true;
    vector<pair<int, int>> tour = init();
    while(improved){
        improved = improve(tour);
    }
    return tour;
}

void save(vector<pair<int, int>>& tour) {
    ofstream output_file("SOL_" + file_name);
    double w = 0;
    for (int i = 0; i < dimension; i++)
        w += distances[i][tour[i].second];
    output_file << w << '\n';
    int cur = 0;
    for (int i = 0; i < dimension; i++) {
        output_file << cur << '\n';
        cur = tour[cur].second;
    }
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    cin >> file_name;
    read_file(file_name);
	vector<pair<int, int>> tour = solve();
    save(tour);
    return 0;
}
