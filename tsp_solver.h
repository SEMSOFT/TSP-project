#include "utils.h"

const int K_NEAREST = 5;

vector <vector<long long>> _distances;
int _dimension;
vector<int> pi, order, diff_degree, color, loop_order;
vector<bool> current_mark;
vector<vector<int>> nearest, edges;

void make_new_tour(vector<pair<int, int>> &tour, set<pair<int, int>> &X, set<pair<int, int>> &Y){ // not effiecient
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
        tour[order[i]] = {order[(i - 1 + _dimension) % _dimension], order[(i + 1) % _dimension]};
        color[order[i]] = i;
    }

//     double sum = 0;
//     for(int i = 0; i < (int)tour.size(); i++){
//         sum += _distances[i][tour[i].first];
//         sum += _distances[i][tour[i].second];
//     }
//     cout << "weight of new tour: " << sum / 2 << endl;

    random_shuffle(loop_order.begin(), loop_order.end());
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

bool chooseX(vector<pair<int, int>> &tour, int t1, int last, double gain, set<pair<int, int>> &X, set<pair<int, int>> &Y);

bool chooseY(vector<pair<int, int>> &tour, int t1, int last, double gain, set<pair<int, int>> &X, set<pair<int, int>> &Y){
    int cnt = 0;
    for(int t2i1: nearest[last]){
        if(cnt++ >= K_NEAREST)
            break;
        double Gi = gain - _distances[last][t2i1];
        pair<int, int> p1;
        if (last < t2i1)
            p1 = {last, t2i1};
        else
            p1 = {t2i1, last};
        if(t2i1 != t1 && X.find(p1) == X.end() && Y.find(p1) == Y.end()){
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

bool chooseX(vector<pair<int, int>> &tour, int t1, int last, double gain, set<pair<int, int>> &X, set<pair<int, int>> &Y){
    if ((int)X.size() > 5)
        return false;
    vector<int> tmp;

    if(_distances[tour[last].first] > _distances[tour[last].second]) {
        tmp.push_back(tour[last].first);
        tmp.push_back(tour[last].second);
    }
    else {
        tmp.push_back(tour[last].second);
        tmp.push_back(tour[last].first);
    }

    for(int i = 0; i < 2; i++){
        int t2i = tmp[i];
        double Gi = gain + _distances[last][t2i];
        pair<int, int> p1;
        if (last < t2i)
            p1 = {last, t2i};
        else
            p1 = {t2i, last};

        if(t2i != t1 && X.find(p1) == X.end() && Y.find(p1) == Y.end()){
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
            if(Gi - _distances[t2i][t1] > 0) {
                make_new_tour(tour, X, Y);
                return true;
            }
            else {
                Y.erase(p2);
                bool result = chooseY(tour, t1, t2i, Gi, X, Y);
                if (result)
                    return true;
                else
                    X.erase(p1);
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
                double gain = _distances[t1][t2] - _distances[t2][t3];

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

vector<pair<int, int>> init(bool doFarthest, bool isHcp) {
    pi.clear();
    order.clear();
    diff_degree.clear();
    color.clear();
    loop_order.clear();
    current_mark.clear();
    nearest.clear();
    edges.clear();
    for (int i = 0; i < _dimension; i++) {
        color.push_back(0);
        diff_degree.push_back(0);
        vector<int> vec;
        edges.push_back(vec);
        current_mark.push_back(false);
        loop_order.push_back(i);
    }

    // cout << "edge transforming" << endl;
    for (int i = 0; i < _dimension; i++)
        pi.push_back(0);
    // pi = edge_transform(_distances);
    //
    // for (int i = 0; i < dimension; i++) {
    //     for (int j = 0; j < dimension; j++) {
    //         if (i != j)
    //             _distances[i][j] += pi[i] + pi[j];
    //     }
    // }

    // cout << "computing alpha nearness" << endl;
    if(isHcp){
        for (int i = 0; i < _dimension; i++) {
            vector<int> nears;
            for (int j = 0; j < _dimension; j++) {
                if (i == j) {
                    continue;
                }
                if(_distances[i][j] == 1){
                    nears.push_back(j);
                }
                nearest.push_back(nears);
            }
        }
    }
    else{
        vector<vector<long long>> a_distances = get_a_nearness(_distances, 0);
        for (int i = 0; i < _dimension; i++) {
            vector<int> nears;
            for (int j = 0; j < _dimension; j++) {
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
    }

    // cout << "constructing the initialize tour" << endl;
    vector<pair<int, int>> resp;
    if (doFarthest)
        resp = get_farthest_insertion_tour(_distances);
    else
        resp = get_random_tour(_dimension);

    order.push_back(0);
    int idx = resp[0].second;
    while (idx) {
        order.push_back(idx);
        idx = resp[idx].second;
    }
    for (int i = 0; i < _dimension; i++) {
        color[order[i]] = i;
    }
    return resp;
}

vector<pair<int, int>> solve(vector<vector<long long>>& distances, bool doFarthest, bool isHcp){
    _distances = distances;
    _dimension = (int)distances.size();
    bool improved = true;
    vector<pair<int, int>> tour = init(doFarthest, isHcp);

    cout << "Init finished!" << endl;
    // cout << "start to improve" << endl;

    while(improved){
        if(isHcp){
            int sum = 0;
            for(int i = 0; i < _dimension; i++){
                sum += _distances[i][tour[i].first];
                sum += _distances[i][tour[i].second];
            }
            if(sum / 2 == _dimension) break;
            for(int i = 0; i < _dimension; i++){
                unsigned seed = std::chrono::system_clock::now()
                        .time_since_epoch()
                        .count();
                shuffle (nearest[i].begin(), nearest[i].end(), std::default_random_engine(seed));
            }
        }
        improved = improve(tour);
    }

    return tour;
}
