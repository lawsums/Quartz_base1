#include <bits/stdc++.h>
using namespace std;


class Solution {
public:
    vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values, vector<vector<string>>& queries) {
        const double eps = 1e-9; // 定义一个误差值, 小于这个值就不用处理
        int nvars = 0; // 有多少个变量
        unordered_map<string, int> variables; // 通过使用哈希表减少时间复杂度

        int n = equations.size();
        // 统计有多少个变量
        for (int i = 0; i < n; i++) { 
            if (variables.find(equations[i][0]) == variables.end()) {
                variables[equations[i][0]] = nvars++;
            }
            if (variables.find(equations[i][1]) == variables.end()) {
                variables[equations[i][1]] = nvars++;
            }
        }

        // 建图
        vector<vector<double>> graph(nvars, vector<double>(nvars, -1.0)); // 由于是需要使用Floyd, 所以不用邻接表而是邻接矩阵 
                                                                          // 1 <= equations.length <= 20
                                                                          // 1 <= Ai.length, Bi.length <= 5
                                                                          
        // 关键修改：初始化对角线，graph[i][i] = 1.0（自身除以自身等于1）
        for (int i = 0; i < nvars; i++) {
            graph[i][i] = 1.0;
        }
        for (int i = 0; i < n; i++) {
            int va = variables[equations[i][0]], vb = variables[equations[i][1]];
            graph[va][vb] = values[i];
            graph[vb][va] = 1.0 / values[i]; // 反向加入边
        }

        // Floyd启动!
        for (int k = 0; k < nvars; k++) {
            for (int i = 0; i < nvars; i++) {
                for (int j = 0; j < nvars; j++) {
                    if (graph[i][k] > eps && graph[k][j] > eps) {
                        graph[i][j] = (graph[i][k] * graph[k][j]);
                    }
                }
            }
        }

        // 通过哈希表快速查找两个变量的值, 如果任意一个不存在就直接写入-1, 否则写入正确答案
        // TODO 如何保证自己除以自己是正确的?
        vector<double> ret;
        for (const auto& q: queries) {
            double result = -1.0;
            if (variables.find(q[0]) != variables.end() && variables.find(q[1]) != variables.end()) {
                int ia = variables[q[0]], ib = variables[q[1]];
                if (graph[ia][ib] > 0) {
                    result = graph[ia][ib];
                }
            }
            ret.push_back(result);
        }
        return ret;
    }
};
