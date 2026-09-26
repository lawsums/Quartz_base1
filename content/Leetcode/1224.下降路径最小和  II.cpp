#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& grid) {
        int n = grid.size();
        if (n == 1) return grid[0][0];

        vector<int> dp(n, 0), new_dp(n, INT_MAX);
        for (int i = 0; i < n; i++) {
            dp[i] = grid[0][i];
        }

        // 循环迭代
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    if (j == k) continue;
                    new_dp[j] = min(new_dp[j], dp[k] + grid[i][j]);
                }
            }

            // 把dp替换成new_dp
            dp = new_dp;
            new_dp.assign(n, INT_MAX);
        }

        return *min_element(dp.begin(), dp.end());
    }
};
