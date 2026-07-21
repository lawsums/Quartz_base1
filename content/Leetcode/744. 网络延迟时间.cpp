#include <bits/stdc++.h>
#include <climits>
#include <queue>
using namespace std;

typedef array<int, 2> aii;

// Dijkstra算法模版
class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int s) {
        vector<vector<aii>> graph(n + 1, vector<aii>());
        for (const auto &edge : times) {
            graph[edge[0]].push_back({edge[1], edge[2]}); 
        }

        vector<int> distance(n + 1, INT_MAX);
        vector<bool> visited(n + 1, false);
        distance[s] = 0; // 初始自己的距离是0

        auto cmp = [](aii& a, aii& b) { return a[1] > b[1]; }; // 小根堆
        priority_queue<aii, vector<aii>, decltype(cmp)> pq(cmp);

        // 将初始的s加入pq, 距离为零
        pq.push({s, 0});
        while (!pq.empty()) {
            int u = pq.top()[0]; pq.pop(); // 自动选取权值最小的边
            if (visited[u]) {
                continue;
            }

            for (const auto &edge : graph[u]) {
                auto [v, w] = edge;
                if (!visited[v] && distance[u] + w < distance[v]) { // 如果值更小的话就更新
                    distance[v] = distance[u] + w; // 说明s到u再到v的路径比原先到v的路径更短
                    pq.push({v, distance[v]});
                }
            }
        }

        int ans = INT_MIN;
        // 寻找所有点中的距离最大值
        for (int i = 1; i <= n; i++) {
            if (distance[i] == INT_MAX) {
                return -1;
            }
            ans = max(ans, distance[i]);
        }
        return ans;
    }

    // 改进版(获取路径)
};


