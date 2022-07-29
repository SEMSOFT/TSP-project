// #pragma GCC optimize("Ofast")
// #pragma GCC optimize ("unroll-loops")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#include <bits/stdc++.h>

using namespace std;

vector <vector<double>> distance;
int dimension;
string file_name, junk;
string test_name, weight_type;
vector pair<double, double> coords;

void calc_euc_distance() {
    for (int i = 0; i < dimension; i++) {
        vector <double> dists;
        for (int j = 0; j < i; j++)
            dists.push_back(distance[j][i]);
        dists.push_back(0);
        for (int j = i + 1; j < dimension; j++) {
            double dist = (coords[i][0] - coords[j][0]) * (coords[i][0] - coords[j][0])
                            + (coords[i][1] - coords[j][1]) * (coords[i][1] - coords[j][1]);
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
        calc_euc_distance();

}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    cin >> file_name;
    read_file(file_name);
	return 0;
}