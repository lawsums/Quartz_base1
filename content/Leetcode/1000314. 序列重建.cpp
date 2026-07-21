#include <bits/stdc++.h>
#include <queue>
using namespace std;

// 如果过程中不止一条路就return false
// 如果记录到最后路径和nums不同也return false
// 否则return true
class Solution {
public:
    bool sequenceReconstruction(vector<int>& nums, vector<vector<int>>& sequences) {
        int n = nums.size();
        vector<int> indegree(n + 1, 0);
        vector<vector<int>> graph(n + 1, vector<int>{});
        for (const auto &seq : sequences) {
            for (int i = 0; i < seq.size() - 1; i++) {
                graph[seq[i]].push_back(seq[i + 1]); // 添加这个边
                indegree[seq[i + 1]]++; // 增加入度
            }
        }

        // 从indegree == 0的地方开始
        vector<int> path;
        auto it = find(indegree.begin() + 1, indegree.end(), 0);
        queue<int> q;
        q.push(it - indegree.begin());
        while (!q.empty()) {
            int u = q.front(); q.pop(); // 获取当前节点, 对所有邻居节点进行indegree--
            path.push_back(u);
            for (const auto &v : graph[u]) {
                if (--indegree[v] == 0) { // 如果等于零就加入队列
                    q.push(v);
                }
            }
            if (q.size() > 1) return false;
        }
         
        return nums == path; // 检查两个数组是否相同
    }

};