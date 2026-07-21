#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    vector<vector<bool>> visit;
    vector<vector<int>> path;
    // 题目通常只需要向右和向下，如果是四连通则保留 4 个方向
    vector<vector<int>> dirs = {{1, 0}, {0, 1}}; 
    int n, m;

public:
    vector<vector<int>> pathWithObstacles(vector<vector<int>>& obstacleGrid) {
        if (obstacleGrid.empty() || obstacleGrid[0][0] == 1) return {};
        
        n = obstacleGrid.size();
        m = obstacleGrid[0].size();
        visit.assign(n, vector<bool>(m, false));
        
        if (dfs(0, 0, obstacleGrid)) {
            return path;
        }
        return {};
    }

    bool dfs(int r, int c, vector<vector<int>>& obstacleGrid) {
        // 修正边界判断：使用 < 而不是 <=
        if (r < 0 || r >= n || c < 0 || c >= m || obstacleGrid[r][c] == 1 || visit[r][c]) {
            return false;
        }

        visit[r][c] = true; // 标记已访问
        path.push_back({r, c});

        // 终点判断：坐标应为 n-1, m-1
        if (r == n - 1 && c == m - 1) return true;

        // 尝试方向
        for (auto& dir : dirs) {
            if (dfs(r + dir[0], c + dir[1], obstacleGrid)) return true;
        }

        // 回溯：注意！不要将 visit[r][c] 设回 false
        // 如果这里无法到达终点，visit 保持 true 可以防止其他路径再次进入这个“死胡同”
        path.pop_back();
        return false;
    }
};
