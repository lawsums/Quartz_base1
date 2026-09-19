#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> shortestDistanceAfterQueries(int n, vector<vector<int>>& queries) {
        // 初始图：i -> i+1
        vector<vector<int>> graph(n);
        for (int i = 0; i + 1 < n; ++i) graph[i].push_back(i + 1);

        // 从 0 出发 BFS，返回 0 到 n-1 的最短距离
        auto bfs = [&]() -> int {
            vector<int> dist(n, -1);
            dist[0] = 0;
            queue<int> q;
            q.push(0);

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                if (u == n - 1) return dist[u];
                for (int v : graph[u]) {
                    if (dist[v] == -1) {
                        dist[v] = dist[u] + 1;
                        q.push(v);
                    }
                }
            }
            return dist[n - 1];
        };

        vector<int> ans;
        for (const auto& query : queries) {
            graph[query[0]].push_back(query[1]);
            ans.push_back(bfs());
        }
        return ans;
    }
};
