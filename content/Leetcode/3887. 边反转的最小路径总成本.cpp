#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minCost(int n, vector<vector<int>>& edges) {
        vector<vector<pair<int, int>>> g(n); // 邻接表
        for (auto& e : edges) {
            int x = e[0], y = e[1], wt = e[2];
            g[x].emplace_back(y, wt);
            g[y].emplace_back(x, wt * 2);
        }

        vector<int> dis(n, INT_MAX);
        // 堆中保存 (起点到节点 x 的最短路长度，节点 x)
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
        dis[0] = 0; // 起点到自己的距离是 0
        pq.emplace(0, 0);

        while (!pq.empty()) {
            auto [dis_x, x] = pq.top();
            pq.pop();
            if (dis_x > dis[x]) { // x 之前出堆过
                continue;
            }
            if (x == n - 1) { // 到达终点
                return dis_x;
            }
            for (auto& [y, wt] : g[x]) {
                auto new_dis_y = dis_x + wt;
                if (new_dis_y < dis[y]) {
                    dis[y] = new_dis_y; // 更新 x 的邻居的最短路
                    // 懒更新堆：只插入数据，不更新堆中数据
                    // 相同节点可能有多个不同的 new_dis_y，除了最小的 new_dis_y，其余值都会触发上面的 continue
                    pq.emplace(new_dis_y, y);
                }
            }
        }

        return -1;
    }
};
