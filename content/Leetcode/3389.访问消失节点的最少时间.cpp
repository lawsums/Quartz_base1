#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    vector<vector<pair<int, int>>> g;

public:
    void build(vector<vector<int>>& edges, int n) {
        g.assign(n, vector<pair<int, int>>{});
        for (auto &edge : edges) {
            auto u = edge[0], v = edge[1], w = edge[2];
            g[u].push_back({v, w});
            g[v].push_back({u, w});  // 无向图：反向边必须也加
        }
    }

    vector<int> minimumTime(int n, vector<vector<int>>& edges, vector<int>& disappear) {
        build(edges, n);

        // g[u] = {(v, w), ...}，点编号 0 ~ n-1
        vector<int> dist(n, INT_MAX);
        vector<bool> visited(n, false);
        // 小根堆，按距离升序
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> heap;

        dist[0] = 0;
        heap.push({0, 0});
        while (!heap.empty()) {
            auto [d, u] = heap.top(); heap.pop();
            if (visited[u]) continue;   // 一个点可能被压入多次，旧的记录直接跳过
            visited[u] = true;          // 出堆即定案，d 就是 0→u 的最短路

            if (d >= disappear[u]) {
                // 到达时该点已消失，作废：记 -1，且不再向外松弛
                dist[u] = -1;
                continue;
            }

            for (auto [v, w] : g[u]) {
                if (!visited[v] && d + w < dist[v]) {
                    dist[v] = d + w;
                    heap.push({dist[v], v});
                }
            }
        }

        // 到最后仍是 INT_MAX 的，说明无法到达
        for (auto &d : dist) {
            if (d == INT_MAX) {
                d = -1;
            }
        }

        return dist;
    }
};
