#include <bits/stdc++.h>
using namespace std;

class Solution {
    vector<vector<int>> g;  // 邻接表
    vector<int> val;        // 节点值（放成员变量，DFS 少传参）

    // dp[u]：以 u 为根的子树中，保证「u → 子树内任意叶子」的路径上
    //        都存在未选节点时，未选节点值之和的最小值
    long long dfs(int u, int parent) {
        long long child_sum = 0;
        bool is_leaf = true;
        for (int v : g[u]) {
            if (v == parent) continue;
            is_leaf = false;
            child_sum += dfs(v, u);  // 选 u 时，每个孩子子树必须各自满足
        }

        if (is_leaf) return val[u];             // 叶子必须不选自己
        return min((long long)val[u], child_sum);  // 不选自己 vs 选自己
    }

public:
    long long maximumScoreAfterOperations(vector<vector<int>>& edges, vector<int>& values) {
        int n = values.size();
        g.assign(n, {});
        val = values;
        for (auto& e : edges) {
            g[e[0]].push_back(e[1]);
            g[e[1]].push_back(e[0]);
        }

        long long total = accumulate(values.begin(), values.end(), 0LL);
        long long min_unselected = dfs(0, -1);  // 最小未选代价
        return total - min_unselected;          // 最大得分 = 总和 - 最小未选代价
    }
};
