class Solution {
public:
    const int MOD = 1e9 + 7;

    int maxProductPath(vector<vector<int>>& grid) {
        int n = grid.size(), m = grid[0].size();
        // DP数组：[i][j][0]最大积，[i][j][1]最小积（用long long避免溢出）
        vector<vector<vector<long long>>> dp(n, vector<vector<long long>>(m, vector<long long>(2)));
        
        // 初始化起点
        dp[0][0][0] = dp[0][0][1] = grid[0][0];

        // 初始化第一列（只能从上边来）
        for (int i = 1; i < n; ++i) {
            long long val = dp[i-1][0][0] * grid[i][0];
            dp[i][0][0] = dp[i][0][1] = val;
        }

        // 初始化第一行（只能从左边来）
        for (int j = 1; j < m; ++j) {
            long long val = dp[0][j-1][0] * grid[0][j];
            dp[0][j][0] = dp[0][j][1] = val;
        }

        // 主循环：同时计算最大/最小积，无需手写num正负分支
        for (int i = 1; i < n; ++i) {
            for (int j = 1; j < m; ++j) {
                long long num = grid[i][j];
                // 拿到上方、左方的最大+最小值（共4个候选值）
                long long up_max = dp[i-1][j][0] * num;
                long long up_min = dp[i-1][j][1] * num;
                long long left_max = dp[i][j-1][0] * num;
                long long left_min = dp[i][j-1][1] * num;
                
                // 新的最大=4个候选值的最大值，新的最小=4个候选值的最小值
                dp[i][j][0] = max({up_max, up_min, left_max, left_min});
                dp[i][j][1] = min({up_max, up_min, left_max, left_min});
            }
        }

        // 最后筛选非负结果，再取模
        long long res = dp[n-1][m-1][0];
        return res < 0 ? -1 : res % MOD;
    }
};
