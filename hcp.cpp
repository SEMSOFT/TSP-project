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
    long long w = 0;
    for (int i = 0; i < dimension; i++)
        w += distances[i][tour[i].first] + distances[i][tour[i].second];
    w /= 2;
    string ans = (w <= zz * dimension) ? "YES" : "NO";
    cout << test_name << ":\n";
    cout << "found: " << ans << ' ' << w << endl;
    cout << "saving the tour" << endl;
    ofstream output_file("hcp_" + file_name);
    int cur = 0;
    for (int i = 0; i < dimension; i++) {
        output_file << cur << '\n';
        cur = tour[cur].second;
    }
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
            dist.push_back(1e4);
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

    for(int i = 0; i < (int)(dimension * sqrt(dimension)); i++){
        int x = rand() % dimension, y = rand() % dimension;
        if(x == y) continue;
        distances[x][y] = zz;
        distances[y][x] = zz;
    }
}

int main() {
    srand(time(0));
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    long long t = time(NULL);
    for (int i = 200; i <= 1000; i++) {
        // cin >> file_name;
        distances.clear();
        file_name = "C:\\Users\\matin\\Desktop\\TSP-project\\FHCPCS\\graph" + to_string(i) + ".hcp";
        read_file(file_name);
	    vector<pair<int, int>> tour = solve(distances, false, true);
        save(tour);
    }
    long long tt = time(NULL);
    cout << tt - t << endl;
    return 0;
}
