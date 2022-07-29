#include <vector>

using namespace std;

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

    return tree;
}
