#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maximalSquare(vector<vector<char>>& grid) {
        int n = grid.size(), m = grid[0].size();
        int ans = 0;
        auto pre = vector<vector<int>>(n, vector<int>(m, 0));

        // 构造二维前缀和
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                pre[i][j] = (grid[i][j] - '0') 
                            + get(pre, i - 1, j) 
                            + get(pre, i, j - 1) 
                            - get(pre, i - 1, j - 1);
            }
        }

        // 对于每个位置进行判断是否有更大的正方形
        for (int i = 0; i < n; i++) { 
            for (int j = 0; j < m; j++) {
                for (int k = min(i, j) + 1; k > ans; k--) {
                    if (pre[i][j] 
                        - get(pre, i - k, j) 
                        - get(pre, i, j - k) 
                        + get(pre, i - k, j - k) == k * k) {
                        ans = k;
                    }
                }
        	}
        }

        return ans * ans;
    }

    int get(vector<vector<int>> &grid, int i, int j) {
        if (i < 0 || j < 0) return 0;
        return grid[i][j];
    }
};

