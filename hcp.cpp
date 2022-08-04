// #pragma GCC optimize("Ofast")
// #pragma GCC optimize ("unroll-loops")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#include <bits/stdc++.h>
#include "tsp_solver.h"

using namespace std;


string file_name, junk;
string test_name;
vector<vector<long long>> distances;
int dimension;

int zz = 1;

void save(vector<pair<int, int>>& tour) {
    // cout << "saving the tour" << endl;
    // ofstream output_file("sol_" + file_name);
    long long w = 0;
    for (int i = 0; i < dimension; i++)
        w += distances[i][tour[i].first] + distances[i][tour[i].second];
    w /= 2;
    string ans = (w <= zz * dimension) ? "YES" : "NO";
    cout << test_name << ": " << ans << ' ' << w << "\n";
    // cout << "found: " << ans << endl;
    // cout << "weight: " << w << endl;
    // output_file << ans << '\n';
    // int cur = 0;
    // for (int i = 0; i < dimension; i++) {
        // output_file << cur << '\n';
        // cur = tour[cur].second;
    // }
}

void read_file(string file_name) {
    // cout << "reading the file..." << endl;
    ifstream input_file(file_name);
    input_file >> junk >> junk >> test_name; // read NAME
    getline(input_file, junk);
    getline(input_file, junk); // read Comment
    getline(input_file, junk); // read Type
    input_file >> junk >> junk >> dimension;
    getline(input_file, junk);
    getline(input_file, junk);
    getline(input_file, junk);

    for (int i = 0; i < dimension; i++) {
        vector <long long> dist;
        for (int j = 0; j < dimension; j++)
            dist.push_back(2);
        dist[i] = 0;
        distances.push_back(dist);
    }

    while (true) {
        int x, y;
        input_file >> x;
        if (x == -1)
            break;
        input_file >> y;
        x--;
        y--;
        distances[x][y] = zz;
        distances[y][x] = zz;
    }
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    long long t = time(NULL);
    for (int i = 1; i <= 10; i++) {
        // cin >> file_name;
        distances.clear();
        file_name = "FHCPCS/graph" + to_string(i) + ".hcp";
        read_file(file_name);
	    vector<pair<int, int>> tour = solve(distances);
        save(tour);
    }
    long long tt = time(NULL);
    cout << tt - t << endl;
    return 0;
}
