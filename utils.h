#include <vector>
#include <cstring>

using namespace std;

vector<vector<int>> subtree;

vector<vector<int>> prim(int v, vector<vector<long long>> &distances, vector<int> &pi){
    int n = distances.size();
    vector<vector<int>> mst;
    bool mark[n];
    int par[n];
    long long mat[n];

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
        mat[i] = distances[t][i] + pi[t] + pi[i];
    }

    for(int i = 0; i < n-2; i++){
        long long mn = 1e18;
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
            if(!mark[j] && distances[last][j] + pi[last] + pi[j] < mat[j]){
                par[j] = last;
                mat[j] = distances[last][j] + pi[last] + pi[j];
            }
        }
    }

    return mst;
}


vector<vector<int>> get_v_tree(int v, vector<vector<long long>> &distances, vector<int> &pi) {
    vector<vector<int>> tree = prim(v, distances, pi);
    long long mn[2];
    mn[0] = mn[1] = 1e18;
    int ind[2];
    int n = distances.size();
    for(int i = 0; i < n; i++){
        if(i != v){
            if(distances[v][i] + pi[v] + pi[i] < mn[0]){
                mn[0] = distances[v][i] + pi[v] + pi[i];
                ind[0] = i;
                continue;
            }
            if(distances[v][i] + pi[v] + pi[i] < mn[1]){
                mn[1] = distances[v][i] + pi[v] + pi[i];
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

vector<int> edge_transform(vector<vector<long long>>& distances) {
    // inja nabayad distances ro avaz konam. bayad copy begiram azash
    int n = distances.size();
    vector <int> final_pi;
    vector <int> pi;
    vector <int> grad;
    for (int i = 0; i < n; i++) {
        pi.push_back(0);
        final_pi.push_back(0);
        grad.push_back(0);
    }
    double w = -1e18;
    double sum_of_pi = 0;

    bool is_tour = false;
    bool first_period = true;
    int period_length = n / 2;
    int repeats = 0;
    int step_size = 1;

    while(!is_tour && step_size && period_length) {
        cout << step_size << ' ' << period_length << ' ' << repeats << endl;
        is_tour = true;
        double tree_weight = 0;
        vector<vector<int>> node = get_v_tree(0, distances, pi);
        for (int i = 0; i < n; i++) {
            grad[i] = (int)node[i].size() - 2;
            is_tour &= (grad[i] == 0);
            for (auto j: node[i]) {
                tree_weight += distances[i][j] + pi[i] + pi[j];
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
    // for (int i = 0; i < n; i++)
        // cout << grad[i] << ' ';
    // cout << endl;

    return pi;
}

vector<pair<int, int>> get_farthest_insertion_tour(vector<vector<long long>>& distances) {
    int dimension = distances.size();
    long long dist[dimension];
    bool mark[dimension];
    vector<pair<int, int>> tour;
    int idx = 1;
    for (int i = 0; i < dimension; i++) {
        dist[i] = distances[0][i];
        if (dist[i] > dist[idx] && i)
            idx = i;
        mark[i] = false;
        tour.push_back({0, 0});
    }
    mark[0] = true;
    for (int turn = 1; turn < dimension; turn++) {
        // idx is choosed
        int cur = 0;
        int who = 0;
        long long w = 1e18;
        while (tour[cur].second != 0) {
            long long cost = distances[cur][idx] + distances[idx][tour[cur].second] - distances[cur][tour[cur].second];
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

void dfs(int v, int par, vector<vector<long long>>& distances, int vroot, long long* dp, vector<vector<int>>& node) {
    subtree[v].push_back(v);
    for (auto u: node[v]) {
        if (u != par && u != vroot) {
            long long w = distances[v][u];
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

vector<vector<long long>> get_a_nearness(vector<vector<long long>> distances, int v) {
    subtree.clear();

    vector<int> pi;
    for (int i = 0; i < (int)distances.size(); i++)
        pi.push_back(0);

    vector<vector<int>> node = get_v_tree(v, distances, pi);

    // edges: the edge set of minimum v-tree
    long long mx = -1e18;
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

    long long dp[dim];
    memset(dp, 0, sizeof(dp));

    for (int i = 0; i < dim; i++) {
        vector<int> vec;
        subtree.push_back(vec);
    }

    dfs(node[v][0], v, distances, v, dp, node);

    return distances;
}
