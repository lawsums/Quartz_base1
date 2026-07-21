#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    int rows[3];
    int cols[3];
    int dig[3];
    int visit[10];

public:
    int numMagicSquaresInside(vector<vector<int>>& grid) {
        int n = grid.size(), m = grid[0].size();
        int ans = 0;
        for (int i = 0; i <= n - 3; i++) {
            for (int j = 0; j <= m - 3; j++) {
                ans += check(i, j, grid) ? 1 : 0;
            }
        }
        return ans;
    }
    
    bool check(int r, int c, vector<vector<int>> &grid) {
        clear();

        // 检查数字范围并记录行和
        for (int i = r; i < r + 3; i++) {
            for (int j = c; j < c + 3; j++) {
                int cur = grid[i][j];
                if (cur == 0 || cur >= 10 || visit[cur]) return false;
                visit[cur] = 1;
                rows[i - r] += cur;
            }
        }

        // 计算列和
        for (int j = c; j < c + 3; j++) {
            for (int i = r; i < r + 3; i++) {
                cols[j - c] += grid[i][j];
            }
        }

        // 计算对角线和
        for (int i = r, j = c, rj = c + 2; i < r + 3; i++, j++, rj--) {
            dig[0] += grid[i][j];
            dig[1] += grid[i][rj];
        }

        // 验证所有和是否相等
        int target = rows[0];
        for (int i = 0; i < 3; i++) {
            if (target != rows[i]) return false;
            if (target != cols[i]) return false;
            if (i < 2 && target != dig[i]) return false;
        }
        return true;
    }

    void clear() {
        for (int i = 0; i < 3; i++) {
            rows[i] = 0;
            cols[i] = 0;
            dig[i] = 0;
        }
        // 修正：使用索引 i 来遍历并清空 visit 数组
        for (int i = 0; i < 10; i++) visit[i] = 0;
    }
};
