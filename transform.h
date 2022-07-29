#include "v_tree.h"

using namespace std;

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
        }
        if (repeats == period_length) {
            step_size /= 2;
            period_length /= 2;
            repeats = period_length;
            first_period = false;
        }
    }

    return pi;
}
