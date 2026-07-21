#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long minimumCost(string source, string target, vector<char>& original, vector<char>& changed, vector<int>& cost) {
        // 初始化26个字母的邻接矩阵，用LLONG_MAX避免int溢出，且适配long long计算
        vector<vector<long long>> graph(26, vector<long long>(26, LLONG_MAX));
        // 自己到自己的成本为0
        for (int i = 0; i < 26; i++) {
            graph[i][i] = 0;
        }

        // 建图：注意**去重**——同一个original->changed可能有多个cost，保留最小的
        int n = original.size();
        for (int i = 0; i < n; i++) {
            int x = original[i] - 'a';
            int y = changed[i] - 'a';
            int c = cost[i];
            graph[x][y] = min((long long)c, graph[x][y]); // 关键：保留最小直接成本
        }

        // Floyd-Warshall核心：k是中转点，必须放在最外层！
        for (int k = 0; k < 26; k++) { // 中转点k：i -> k -> j
            for (int i = 0; i < 26; i++) { // 起点i
                for (int j = 0; j < 26; j++) { // 终点j
                    // 关键：判断i->k和k->j是否可达（避免LLONG_MAX相加溢出）
                    if (graph[i][k] != LLONG_MAX && graph[k][j] != LLONG_MAX) {
                        graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
                    }
                }
            }
        }

        // 计算总转换成本
        long long ans = 0;
        int m = source.size();
        for (int i = 0; i < m; i++) {
            int x = source[i] - 'a';
            int y = target[i] - 'a';
            if (graph[x][y] == LLONG_MAX) { // 无法转换，返回-1
                return -1;
            }
            ans += graph[x][y];
        }
        return ans;
    }
};
