#include <vector>
#include <cstring>
#include "v_tree.h"

using namespace std;

vector<vector<int>> subtree;

void dfs(int v, int par, vector<vector<double>>& distances, int vroot, double* dp, vector<int>* node) {
    subtree[v].push_back(v);
    for (auto u: node[v]) {
        if (u != par && u != vroot) {
            dfs(u, v, distances, vroot, dp, node);
            double w = distances[v][u];
            for (auto p: subtree[u]) {
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
    vector<int> node[] = get_v_tree(distances, v);
    // edges: the edge set of minimum v-tree
    double mx = 0;
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
        subtree[i] = vec;
    }

    dfs(node[v][0], v, distances, v, dp, node);
}
