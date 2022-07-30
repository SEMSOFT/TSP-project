#include <vector>
#include <cstring>

using namespace std;

vector<vector<int>> subtree;

vector<vector<int>> prim(int v, vector<vector<double>> &distances){
    int n = distances.size();
    vector<vector<int>> mst;
    bool mark[n];
    int par[n];
    double mat[n];

    for(int i = 0; i < n; i++){
        vector<int> tmp;
        mst.push_back(tmp);
    }

    fill(mat, mat + n, 1e18);
    fill(mark, mark + n, 0);
    mark[v] = 1;

    int t = 0;
    if(v == 0){
        t = 1;
    }

    mark[t] = 1;
    for(int i = 0; i < n; i++){
        if(i != t)
            par[i] = t;
        mat[i] = distances[t][i];
    }

    for(int i = 0; i < n-2; i++){
        double mn = 1e18;
        int last = -1;
        for(int j = 0; j < n; j++){
            if(!mark[j] && mat[j] < mn){
                last = j;
                mn = mat[j];
            }
        }

        mark[last] = 1;
        mst[last].push_back(par[last]);
        mst[par[last]].push_back(last);

        for(int j = 0; j < n; j++){
            if(!mark[j] && distances[last][j] < mat[j]){
                par[j] = last;
                mat[j] = distances[last][j];
            }
        }
    }

    return mst;
}


vector<vector<int>> get_v_tree(int v, vector<vector<double>> &distances) {
    vector<vector<int>> tree = prim(v, distances);
    double mn[2] = {1e18, 1e18};
    int ind[2];
    int n = distances.size();
    for(int i = 0; i < n; i++){
        if(i != v){
            if(distances[v][i] < mn[0]){
                mn[0] = distances[v][i];
                ind[0] = i;
                continue;
            }
            if(distances[v][i] < mn[1]){
                mn[1] = distances[v][i];
                ind[1] = i;
                continue;
            }
        }
    }

    tree[v].push_back(ind[0]);
    tree[v].push_back(ind[1]);
    tree[ind[0]].push_back(v);
    tree[ind[1]].push_back(v);

//    double sum = 0;
//    for(int i = 0; i < n; i++){
//        for(int j: tree[i]){
//            sum += distances[i][j];
//        }
//    }
//    cout << "weight of " << v << "-tree: " << sum / 2 << endl;

    return tree;
}

vector<int> edge_transform(vector<vector<double>>& distances) {
    int n = distances.size();
    vector <int> final_pi;
    vector <int> pi;
    for (int i = 0; i < n; i++) {
        pi.push_back(0);
        final_pi.push_back(0);
    }
    int grad[n];
    double w = -1e18;
    double sum_of_pi = 0;

    bool is_tour = false;
    bool first_period = true;
    int period_length = n / 2;
    int repeats = 0;
    int step_size = 1;

    while(!is_tour && step_size && period_length) {
        is_tour = true;
        double tree_weight = 0;
        vector<vector<int>> node = get_v_tree(1, distances);
        for (int i = 0; i < n; i++) {
            grad[i] = (int)node[i].size() - 2;
            is_tour &= (grad[i] == 0);
            for (auto j: node[i]) {
                tree_weight += distances[i][j];
            }
        }
        tree_weight /= 2;
        tree_weight -= 2 * sum_of_pi;

        repeats++;

        if (tree_weight > w) {
            w = tree_weight;
            for (int j = 0; j < n; j++)
                final_pi[j] = pi[j];
            if (repeats == period_length)
                period_length *= 2;
            if (first_period)
                step_size *= 2;
        }

        // pi_(k+1) = pi_k + t_k * grad_k
        for (int i = 0; i < n; i++) {
            pi[i] += step_size * grad[i];
            sum_of_pi += step_size * grad[i];
        }
        if (repeats == period_length) {
            step_size /= 2;
            period_length /= 2;
            repeats = 0;
            first_period = false;
        }
    }

    return pi;
}

vector<pair<int, int>> get_farthest_insertion_tour(vector<vector<double>>& distances) {
    int dimension = distances.size();
    double dist[dimension];
    bool mark[dimension];
    vector<pair<int, int>> tour;
    int idx = 1;
    for (int i = 0; i < dimension; i++) {
        dist[i] = distances[0][i];
        if (dist[i] > dist[idx])
            idx = i;
        mark[i] = false;
        tour.push_back({0, 0});
    }
    mark[0] = true;
    for (int turn = 1; turn < dimension; turn++) {
        // idx is choosed
        int cur = 0;
        int who = 0;
        double w = 1e18;
        while (tour[cur].second != 0) {
            double cost = distances[cur][idx] + distances[idx][tour[cur].second] - distances[cur][tour[cur].second];
            if (cost < w) {
                who = cur;
                w = cost;
            }
            cur = tour[cur].second;
        }
        tour[idx] = {who, tour[who].second};
        tour[tour[who].second] = {idx, tour[tour[who].second].second};
        tour[who] = {tour[who].first, idx};

        mark[idx] = true;
        int z = idx;
        for (int i = 0; i < dimension; i++) {
            if (!mark[i]) {
                if (z == idx)
                    idx = i;
                dist[i] = max(dist[i], distances[z][i]);
                if (dist[i] > dist[idx])
                    idx = i;
            }
        }
    }
    return tour;
}

void dfs(int v, int par, vector<vector<double>>& distances, int vroot, double* dp, vector<vector<int>>& node) {
    subtree[v].push_back(v);
    for (auto u: node[v]) {
        if (u != par && u != vroot) {
            double w = distances[v][u];
            dfs(u, v, distances, vroot, dp, node);
            dp[u] = w;
            for (auto p: subtree[u]) {
                if (p != u)
                    dp[p] = max(dp[p], w);
                distances[v][p] -= dp[p];
                distances[p][v] -= dp[p];
            }
        }
    }
    for (auto u: node[v]) {
        if (u == par || u == vroot)
            continue;
        for (auto p: node[v]) {
            if (u == p || p == par || p == vroot)
                continue;
            for (auto v1: subtree[u]) {
                for (auto v2: subtree[p]) {
                    distances[v1][v2] -= max(dp[v1], dp[v2]);
                    distances[v2][v1] = distances[v1][v2];
                }
            }
        }
        subtree[v].insert(subtree[v].end(), subtree[u].begin(), subtree[u].end());
        subtree[u].clear();
    }
}

void get_a_nearness(vector<vector<double>>& distances, int v) {
    subtree.clear();

    vector<vector<int>> node = get_v_tree(v, distances);

    // edges: the edge set of minimum v-tree
    double mx = -1e18;
    int dim = distances.size();

    // calculate a_nearness for edges of the format (v, .)
    for (auto i: node[v]) {
        if (mx < distances[v][i])
            mx = distances[v][i];
    }
    for (int i = 0; i < dim; i++) {
        distances[v][i] = distances[v][i] - mx;
        if (distances[v][i] < 0)
            distances[v][i] = 0;
        distances[i][v] = distances[v][i];
    }

    double dp[dim];
    memset(dp, 0, sizeof(dp));

    for (int i = 0; i < dim; i++) {
        vector<int> vec;
        subtree.push_back(vec);
    }

    dfs(node[v][0], v, distances, v, dp, node);
}
