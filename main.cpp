// #pragma GCC optimize("Ofast")
// #pragma GCC optimize ("unroll-loops")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

const int K_NEAREST = 5;

vector <vector<double>> distances;
int dimension;
string file_name, junk;
string test_name, weight_type;
vector<int> pi;
vector <pair<double, double>> coords;
vector<vector<int>> nearest;

int my_counter;

void check_tour(int v, bool* mark, vector<vector<int>>& node) {
    mark[v] = true;
    my_counter++;
    if (!mark[node[v][0]])
        check_tour(node[v][0], mark, node);
    if (!mark[node[v][1]])
        check_tour(node[v][1], mark, node);
}

bool is_tour(vector<pair<int, int>> &tour, set<pair<int, int>> &X, set<pair<int, int>> &Y) {
    my_counter = 0;
    vector<vector<int>> node;
    for (int i = 0; i < dimension; i++) {
        vector<int> adj;
        adj.push_back(tour[i].first);
        adj.push_back(tour[i].second);
        node.push_back(adj);
    }
    set<pair<int, int>>::iterator it;
    for (it = X.begin(); it != X.end(); ++it) {
        pair <int, int> e = *it;
        for (int i = 0; i < (int)node[e.first].size(); i++) {
            if (node[e.first][i] == e.second) {
                swap(node[e.first][i], node[e.first][node[e.first].size() - 1]);
                node[e.first].pop_back();
                break;
            }
        }

        for (int i = 0; i < (int)node[e.second].size(); i++) {
            if (node[e.second][i] == e.first) {
                swap(node[e.second][i], node[e.second][node[e.second].size() - 1]);
                node[e.second].pop_back();
                break;
            }
        }
    }

    for (it = Y.begin(); it != Y.end(); ++it) {
        pair <int, int> e = *it;
        node[e.first].push_back(e.second);
        node[e.second].push_back(e.first);
    }

    for (int i = 0; i < dimension; i++) {
        if (node[i].size() != 2)
            return false;
    }

    bool mark[dimension];
    memset(mark, 0, sizeof(mark));
    check_tour(0, mark, node);

    return my_counter == dimension;
}

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
        distances.push_back(dists);
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

bool chooseX(vector<pair<int, int>> &tour, int t1, int last, double gain, set<pair<int, int>> &X, set<pair<int, int>> &Y);

bool chooseY(vector<pair<int, int>> &tour, int t1, int last, double gain, set<pair<int, int>> &X, set<pair<int, int>> &Y){
    for(int t2i1: nearest[last]){
        double Gi = gain - distances[last][t2i1];
        pair<int, int> p1 = {last, t2i1}, p2 = {t2i1, last};
        if(t2i1 != t1 && !X.contains(p1) && !X.contains(p2) && !Y.contains(p1) && !Y.contains(p2)){
            Y.insert(p1);
            bool check = chooseX(tour, t1, t2i1, Gi , X, Y);
            if(check){
                return true;
            }
            Y.erase(p1);
        }
    }
    return false;
}

void make_new_tour(vector<pair<int, int>> &tour, set<pair<int, int>> &X, set<pair<int, int>> &Y){ // not effiecient
    cout << "found an improvement!" << endl;
    my_counter = 0;
    vector<vector<int>> node;
    for (int i = 0; i < dimension; i++) {
        vector<int> adj;
        adj.push_back(tour[i].first);
        adj.push_back(tour[i].second);
        node.push_back(adj);
    }
    set<pair<int, int>>::iterator it;
    for (it = X.begin(); it != X.end(); ++it) {
        pair <int, int> e = *it;
        for (int i = 0; i < (int)node[e.first].size(); i++) {
            if (node[e.first][i] == e.second) {
                swap(node[e.first][i], node[e.first][node[e.first].size() - 1]);
                node[e.first].pop_back();
                break;
            }
        }

        for (int i = 0; i < (int)node[e.second].size(); i++) {
            if (node[e.second][i] == e.first) {
                swap(node[e.second][i], node[e.second][node[e.second].size() - 1]);
                node[e.second].pop_back();
                break;
            }
        }
    }

    for (it = Y.begin(); it != Y.end(); ++it) {
        pair <int, int> e = *it;
        node[e.first].push_back(e.second);
        node[e.second].push_back(e.first);
    }

    int cur = 0;
    vector<int> order;
    order.push_back(0);

    bool mark[dimension];
    memset(mark, 0, sizeof(mark));
    mark[0] = true;

    for (int i = 1; i < dimension; i++) {
        if (!mark[node[cur][0]]) {
            cur = node[cur][0];
            mark[cur] = true;
            order.push_back(cur);
        }
        else if (!mark[node[cur][1]]) {
            cur = node[cur][1];
            mark[cur] = true;
            order.push_back(cur);
        }
        else {
            cout << "YOUR CODE HAS BUUUUG!";
        }
    }

    for (int i = 0; i < dimension; i++)
        tour[order[i]] = {order[(i - 1 + dimension) % dimension], order[(i + 1) % dimension]};

    double sum = 0;
    for(int i = 0; i < (int)tour.size(); i++){
        sum += distances[i][tour[i].first];
        sum += distances[i][tour[i].second];
    }
    cout << "weight of new tour: " << sum / 2 << endl;
}


bool chooseX(vector<pair<int, int>> &tour, int t1, int last, double gain, set<pair<int, int>> &X, set<pair<int, int>> &Y){
    vector<int> tmp;
    if(X.size() == 3){
        if(distances[tour[last].first] > distances[tour[last].second]){
            tmp.push_back(tour[last].first);
//             tmp.push_back(tour[last].second); // IS THIS TRUE?
        }
        else{
            tmp.push_back(tour[last].second);
//            tmp.push_back(tour[last].second); // IS THIS TRUE?
        }
    } else{
        tmp.push_back(tour[last].first);
        tmp.push_back(tour[last].second);
    }

    for(int i = 0; i < (int)tmp.size(); i++){
        int t2i = tmp[i];
        double Gi = gain + distances[last][t2i];
        pair<int, int> p1 = {last, t2i}, p2 = {t2i, last};

        if(t2i != t1 && !X.contains(p1) && !X.contains(p2) && !Y.contains(p1) && !Y.contains(p2)){
            X.insert(p1);
            Y.insert({t2i, t1});
            if(!is_tour(tour, X, Y)){
                X.erase(p1);
                Y.erase({t2i, t1});
                continue;
            }
            if(Gi - distances[t2i][t1] > 0){
                make_new_tour(tour, X, Y);
                return true;
            }
            else{
                Y.erase({t2i, t1});
                return chooseY(tour, t1, t2i, Gi, X, Y);
            }
        }
    }
    return false;
}

bool improve(vector<pair<int, int>> &tour){
    int n = tour.size();
    for(int t1 = 0; t1 < n; t1++){
        int tmp[2] = {tour[t1].first, tour[t1].second};
        for(int i = 0; i < 2; i++){
            int t2 = tmp[i];
            set<pair<int, int>> X;
            X.insert({t1, t2});

            for(int t3 = 0; t3 < n; t3++){
                if(t3 == tour[t2].first || t3 == tour[t2].second || t3 == t2)
                    continue;
                set<pair<int, int>> Y;
                Y.insert({t2, t3});
                double gain = distances[t1][t2] - distances[t2][t3];

                if(gain > 0){
                    if(chooseX(tour, t1, t3, gain, X, Y)){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

vector<pair<int, int>> init() {
    cout << "edge transforming" << endl;
    pi = edge_transform(distances);

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (i != j)
                distances[i][j] += pi[i] + pi[j];
        }
    }

    cout << "computing alpha nearness" << endl;
    get_a_nearness(distances, 1);

    for (int i = 0; i < dimension; i++) {
        vector<int> nears;
        for (int j = 0; j < dimension; j++) {
            if (i == j) {
                continue;
            }
            nears.push_back(j);
            for (int k = (int)nears.size() - 1; k; k--) {
                if (distances[i][nears[k]] >= distances[i][nears[k - 1]])
                    break;
                swap(nears[k - 1], nears[k]);
            }
            if (nears.size() > K_NEAREST)
                nears.pop_back();
        }
        nearest.push_back(nears);
    }

    cout << "constructing the initialize tour" << endl;

    return get_farthest_insertion_tour(distances);
}

vector<pair<int, int>> solve(){
    bool improved = true;
    vector<pair<int, int>> tour = init();

    cout << "Init finished!" << endl;
    cout << "start to improve" << endl;

    while(improved){
        improved = improve(tour);
    }
    return tour;
}

void save(vector<pair<int, int>>& tour) {
    cout << "saving the tour" << endl;
    ofstream output_file("sol_" + file_name);
    double w = 0;
    for (int i = 0; i < dimension; i++) {
        double di = (coords[i].first - coords[tour[i].second].first) * (coords[i].first - coords[tour[i].second].first)
                  + (coords[i].second - coords[tour[i].second].second) * (coords[i].second - coords[tour[i].second].second);
        w += sqrt(di);
    }
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
