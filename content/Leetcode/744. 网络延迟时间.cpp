#include <bits/stdc++.h>
#include <climits>
#include <queue>
using namespace std;

// Dijkstra 模板题：LeetCode 743. 网络延迟时间
// 单源最短路（源点 k），边权非负 → 小根堆优化的 Dijkstra
class Solution {
private:
    vector<vector<pair<int, int>>> g;

public:
    void build(vector<vector<int>>& times, int n) {
        g.assign(n + 1, vector<pair<int, int>>{});
        for (auto &time : times) {
            auto u = time[0], v = time[1], w = time[2];
            g[u].push_back({v, w});
        }
    }

    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        build(times, n);

        // g[u] = {(v, w), ...}, 点编号 1 ~ n
        vector<int> dist(n + 1, INT_MAX);
        vector<bool> visited(n + 1, false);
        // 小根堆, 按距离升序
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> heap;

        int start = k;
        dist[start] = 0;
        heap.push({0, start});
        while (!heap.empty()) {
            auto [d, u] = heap.top(); heap.pop();
            if (visited[u]) continue;   // 一个点可能被压入多次, 旧的记录直接跳过
            visited[u] = true;          // 出堆即定案, d 就是 start->u 的最短路
            for (auto [v, w] : g[u]) {
                if (!visited[v] && d + w < dist[v]) {
                    dist[v] = d + w;
                    heap.push({dist[v], v});
                }
            }
        }

        // 本题要求: 所有点都收到信号 -> 取所有最短距离的最大值; 有不可达点则返回 -1
        int ans = *max_element(dist.begin() + 1, dist.end());
        return ans == INT_MAX ? -1 : ans;
    }
};
