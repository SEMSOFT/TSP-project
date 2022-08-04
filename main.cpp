// #pragma GCC optimize("Ofast")
// #pragma GCC optimize ("unroll-loops")
// #pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#include <bits/stdc++.h>
#include "utils.h"

using namespace std;

const int K_NEAREST = 5;
const double PI = 3.14159265358979323846;

vector <vector<long long>> distances;
int dimension;
string file_name, junk;
string test_name, weight_type;
vector<int> pi, order, diff_degree, color, loop_order;
vector <pair<double, double>> coords;
vector<bool> current_mark;
vector<vector<int>> nearest, edges;

int improvement_counter;

void save(vector<pair<int, int>>& tour) {
    cout << "saving the tour" << endl;
    ofstream output_file("sol_" + file_name);
    double w = 0;
    for (int i = 0; i < dimension; i++)
        w += distances[i][tour[i].first] + distances[i][tour[i].second];
    w /= 2;
    output_file << w << '\n';
    int cur = 0;
    for (int i = 0; i < dimension; i++) {
        output_file << cur << '\n';
        cur = tour[cur].second;
    }
}

bool is_tour(vector<pair<int, int>> &tour, set<pair<int, int>> &X, set<pair<int, int>> &Y) {
    set<pair<int, int>>::iterator it;
    vector<int> in_order_vertices;
    for (it = Y.begin(); it != Y.end(); it++) {
        diff_degree[(*it).first]++;
        diff_degree[(*it).second]++;
        if (!current_mark[(*it).first]) {
            in_order_vertices.push_back((*it).first);
            current_mark[(*it).first] = true;
        }
        if (!current_mark[(*it).second]) {
            in_order_vertices.push_back((*it).second);
            current_mark[(*it).second] = true;
        }
    }

    for (it = X.begin(); it != X.end(); it++) {
        diff_degree[(*it).first]--;
        diff_degree[(*it).second]--;
        if (!current_mark[(*it).first]) {
            in_order_vertices.push_back((*it).first);
            current_mark[(*it).first] = true;
        }
        if (!current_mark[(*it).second]) {
            in_order_vertices.push_back((*it).second);
            current_mark[(*it).second] = true;
        }
    }

    bool is_all_zero = true;
    for (auto i: in_order_vertices) {
        is_all_zero &= diff_degree[i] == 0;
        diff_degree[i] = 0;
        current_mark[i] = false;
    }

    if (!is_all_zero)
        return false;

    sort(in_order_vertices.begin(), in_order_vertices.end(), [] (const int& lhs, const int& rhs) {
        return color[lhs] < color[rhs];
    });

    multiset <pair<int, int>> st;
    int ssz = (int)in_order_vertices.size();
    for (int i = 0; i < ssz; i++) {
        int u = in_order_vertices[i];
        int v = in_order_vertices[i == (ssz - 1) ? 0 : (i + 1)];
        if (v < u)
            swap(u, v);
        st.insert({u, v});
    }

    for (it = X.begin(); it != X.end(); it++) {
        int u = (*it).first;
        int v = (*it).second;
        if (v < u)
            swap(u, v);
        st.erase(st.find({u, v}));
    }

    for (it = Y.begin(); it != Y.end(); it++) {
        int u = (*it).first;
        int v = (*it).second;
        if (v < u)
            swap(u, v);
        st.insert({u, v});
    }

    for (it = st.begin(); it != st.end(); it++) {
        int u = (*it).first;
        int v = (*it).second;
        edges[u].push_back(v);
        edges[v].push_back(u);
    }

    int last_node = in_order_vertices[0];
    int tmp = edges[in_order_vertices[0]][1];
    int cur_cnt = 1;
    while (tmp != in_order_vertices[0]) {
        cur_cnt++;
        int u = edges[tmp][0];
        int v = edges[tmp][1];
        if (last_node != u) {
            last_node = tmp;
            tmp = u;
        }
        else {
            last_node = tmp;
            tmp = v;
        }
    }

    for (auto i: in_order_vertices)
        edges[i].clear();

    return cur_cnt == ssz;
}

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
        long long lat1 = PI * (deg + 5.0 * mn / 3.0) / 180.0;
        deg = coords[i].second + 0.5;
        mn = coords[i].second - deg;
        long long long1 = PI * (deg + 5.0 * mn / 3.0) / 180.0;

        vector <long long> dists;
        for (int j = 0; j < i; j++)
            dists.push_back(distances[j][i]);
        dists.push_back(0);
        for (int j = i + 1; j < dimension; j++) {
            deg = coords[j].first + 0.5;
            mn = coords[j].first - deg;
            long long lat2 = PI * (deg + 5.0 * mn / 3.0) / 180.0;
            deg = coords[j].second + 0.5;
            mn = coords[j].second - deg;
            long long long2 = PI * (deg + 5.0 * mn / 3.0) / 180.0;

            double a = cos(long1 - long2);
            double b = cos(lat1 - lat2);
            double c = cos(lat1 + lat2);
            dists.push_back((long long)(radius * acos(0.5 * ((1 + a) * b - (1 - a) * c)) + 1.0));
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

bool chooseX(vector<pair<int, int>> &tour, int t1, int last, double gain, set<pair<int, int>> &X, set<pair<int, int>> &Y);

bool chooseY(vector<pair<int, int>> &tour, int t1, int last, double gain, set<pair<int, int>> &X, set<pair<int, int>> &Y){
    for(int t2i1: nearest[last]){
        double Gi = gain - distances[last][t2i1];
        pair<int, int> p1;
        if (last < t2i1)
            p1 = {last, t2i1};
        else
            p1 = {t2i1, last};
        if(t2i1 != t1 && !X.contains(p1) && !Y.contains(p1)){
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
    // cout << "found an improvement!" << endl;

    set<pair<int, int>>::iterator it;
    for (it = X.begin(); it != X.end(); ++it) {
        pair <int, int> e = *it;
        if (tour[e.first].first == e.second)
            tour[e.first].first = -1;
        else
            tour[e.first].second = -1;

        if (tour[e.second].first == e.first)
            tour[e.second].first = -1;
        else
            tour[e.second].second = -1;
    }

    for (it = Y.begin(); it != Y.end(); ++it) {
        pair <int, int> e = *it;
        if (tour[e.first].first == -1)
            tour[e.first].first = e.second;
        else
            tour[e.first].second = e.second;

        if (tour[e.second].first == -1)
            tour[e.second].first = e.first;
        else
            tour[e.second].second = e.first;
    }

    int last = 0;
    int cur = tour[0].first;
    order.clear();
    order.push_back(0);

    while (cur) {
        order.push_back(cur);
        if (tour[cur].first != last) {
            last = cur;
            cur = tour[cur].first;
        }
        else {
            last = cur;
            cur = tour[cur].second;
        }
    }

    for (int i = 0; i < (int)tour.size(); i++) {
        tour[order[i]] = {order[(i - 1 + dimension) % dimension], order[(i + 1) % dimension]};
        color[order[i]] = i;
    }

    // double sum = 0;
    // for(int i = 0; i < (int)tour.size(); i++){
    //     sum += distances[i][tour[i].first];
    //     sum += distances[i][tour[i].second];
    // }
    // cout << "weight of new tour: " << sum / 2 << endl;

    // improvement_counter++;
    // if (improvement_counter == 5) {
    random_shuffle(loop_order.begin(), loop_order.end());
        // improvement_counter = 0;
    // }
}

bool chooseX(vector<pair<int, int>> &tour, int t1, int last, double gain, set<pair<int, int>> &X, set<pair<int, int>> &Y){
    vector<int> tmp;
    if(X.size() == 3){
        if(distances[tour[last].first] > distances[tour[last].second]){
            tmp.push_back(tour[last].first);
        }
        else{
            tmp.push_back(tour[last].second);
        }
    } else{
        tmp.push_back(tour[last].first);
        tmp.push_back(tour[last].second);
    }

    for(int i = 0; i < (int)tmp.size(); i++){
        int t2i = tmp[i];
        double Gi = gain + distances[last][t2i];
        pair<int, int> p1;
        if (last < t2i)
            p1 = {last, t2i};
        else
            p1 = {t2i, last};

        if(t2i != t1 && !X.contains(p1) && !Y.contains(p1)){
            X.insert(p1);
            pair<int, int> p2 = {t2i, t1};
            if (t1 < t2i)
                p2 = {t1, t2i};
            Y.insert(p2);
            if(!is_tour(tour, X, Y)){
                X.erase(p1);
                Y.erase(p2);
                continue;
            }
            if(Gi - distances[t2i][t1] > 0){
                make_new_tour(tour, X, Y);
                return true;
            }
            else{
                Y.erase(p2);
                return chooseY(tour, t1, t2i, Gi, X, Y);
            }
        }
    }
    return false;
}

bool improve(vector<pair<int, int>> &tour){
    for(auto t1: loop_order){
        int tmp[2] = {tour[t1].first, tour[t1].second};
        for(int i = 0; i < 2; i++){
            int t2 = tmp[i];
            set<pair<int, int>> X;
            pair<int, int> p1 = {t1, t2};
            if (t1 > t2)
                p1 = {t2, t1};
            X.insert(p1);

            for(auto t3: loop_order){
                if(t3 == tour[t2].first || t3 == tour[t2].second || t3 == t2)
                    continue;
                set<pair<int, int>> Y;
                pair<int, int> p2 = {t2, t3};
                if (t3 < t2)
                    p2 = {t3, t2};
                Y.insert(p2);
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
    for (int i = 0; i < dimension; i++) {
        color.push_back(0);
        diff_degree.push_back(0);
        vector<int> vec;
        edges.push_back(vec);
        current_mark.push_back(false);
        loop_order.push_back(i);
    }

    cout << "edge transforming" << endl;
    // pi = edge_transform(distances);
    for (int i = 0; i < dimension; i++)
        pi.push_back(0);

    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            if (i != j)
                distances[i][j] += pi[i] + pi[j];
        }
    }

    cout << "computing alpha nearness" << endl;
    vector<vector<long long>> a_distances = get_a_nearness(distances, 0);

    for (int i = 0; i < dimension; i++) {
        vector<int> nears;
        for (int j = 0; j < dimension; j++) {
            if (i == j) {
                continue;
            }
            nears.push_back(j);
            for (int k = (int)nears.size() - 1; k; k--) {
                if (a_distances[i][nears[k]] >= a_distances[i][nears[k - 1]])
                    break;
                swap(nears[k - 1], nears[k]);
            }
            if (nears.size() > K_NEAREST)
                nears.pop_back();
        }
        nearest.push_back(nears);
    }

    cout << "constructing the initialize tour" << endl;
    vector<pair<int, int>> resp = get_farthest_insertion_tour(a_distances);
    order.push_back(0);
    int idx = resp[0].second;
    while (idx) {
        order.push_back(idx);
        idx = resp[idx].second;
    }
    for (int i = 0; i < dimension; i++) {
        color[order[i]] = i;
    }
    return resp;
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

int main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    cin >> file_name;
    long long t = time(NULL);
    read_file(file_name);
	vector<pair<int, int>> tour = solve();
    save(tour);
    long long tt = time(NULL);
    cout << tt - t << endl;
    return 0;
}
