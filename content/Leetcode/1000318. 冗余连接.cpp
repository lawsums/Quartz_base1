#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    // 并查集核心：父节点数组
    vector<int> parent;
    // 查找（带路径压缩，优化查询效率）
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩：让x直接指向根节点
        }
        return parent[x];
    }
    // 合并（带按秩合并，优化合并效率）
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX != rootY) {
            parent[rootY] = rootX; // 合并两个集合（rootY指向rootX）
        }
    }

public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        return f1(edges);
    }

    // 并查集做法
    vector<int> f1(vector<vector<int>>& edges) {
        int n = edges.size(); // 题目提示n == edges.length（节点数=边数，因为树+1条边）
        parent.resize(n + 1); // 节点值1~n，数组索引0不用
        
        // 初始化并查集：每个节点父节点是自己
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
        }
        
        // 遍历每条边，判断是否形成环
        for (auto& edge : edges) {
            int a = edge[0], b = edge[1];
            if (find(a) == find(b)) {
                // 两个节点已连通，当前边是冗余边（最后出现的，直接返回）
                return edge;
            }
            // 未连通，合并两个集合
            unite(a, b);
        }
        
        return {-1, -1}; // 题目保证有解，不会走到这里
    }


    // 我的方法, 错误在只考虑连通性没有考虑成环 
    vector<int> f2(vector<vector<int>>& edges) {
        int n = 0;
        for (auto &e : edges) {
            n = max(n, max(e[0], e[1])); // 更新当前最大节点编号
        }

        // 统计入度
        vector<int> indegree(n + 1, 0);
        for (const auto& edge : edges) {
            indegree[edge[0]]++;
            indegree[edge[1]]++;
        }
        
        // 倒着遍历边, 如果哪个边删除了不会导致入度降为0, 就直接return
        for (int i = edges.size() - 1; i >= 0; i--) {
            if (indegree[edges[i][0]] == 1 || indegree[edges[i][1]] == 1) continue; 
            return edges[i];
        }

        return {-1, -1}; // 不会到达这个地方
    }
};
