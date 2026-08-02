#include <bits/stdc++.h>
using namespace std;

// ============================================================
// 174. 地下城游戏 (Dungeon Game)
// ============================================================
// 两种解法：
//   方法一（主解法）：逆向 DP，O(m*n)，从终点倒推
//   方法二（备选）  ：二分答案 + 正向 DP 判可行性，O(log(INT_MAX)*m*n)
// ============================================================

class Solution {
public:
    // ----------------------------------------------------------
    // 方法一：逆向 DP（标准解法，空间优化 O(m)）
    // ----------------------------------------------------------
    // dp[i][j] = 从 (i,j) 到终点所需的最小初始血量
    // 转移：dp[i][j] = max(1, min(dp[i+1][j], dp[i][j+1]) - dungeon[i][j])
    // ----------------------------------------------------------
    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        int n = dungeon.size(), m = dungeon[0].size();
        vector<int> dp(m, 0);

        for (int i = n - 1; i >= 0; --i) {
            for (int j = m - 1; j >= 0; --j) {
                if (i == n - 1 && j == m - 1) {
                    dp[j] = max(1, 1 - dungeon[i][j]);
                } else if (i == n - 1) {
                    dp[j] = max(1, dp[j + 1] - dungeon[i][j]);
                } else if (j == m - 1) {
                    dp[j] = max(1, dp[j] - dungeon[i][j]);
                } else {
                    dp[j] = max(1, min(dp[j], dp[j + 1]) - dungeon[i][j]);
                }
            }
        }
        return dp[0];
    }

    // ----------------------------------------------------------
    // 方法二：二分答案 + 正向 DP 判可行性
    // ----------------------------------------------------------
    // 对初始血量 HP 做二分搜索，对每个 HP 用正向 DP 判断能否走到终点
    // 注意：这里的正向 DP 只做可行性判断，不是求最优解
    // ----------------------------------------------------------
    int calculateMinimumHP_BinarySearch(vector<vector<int>>& dungeon) {
        int n = dungeon.size(), m = dungeon[0].size();

        auto canReach = [&](int initHP) -> bool {
            vector<vector<int>> dp(n, vector<int>(m, -1));
            dp[0][0] = initHP + dungeon[0][0];
            if (dp[0][0] <= 0) return false;

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (i == 0 && j == 0) continue;
                    int best = -1;
                    if (i > 0 && dp[i - 1][j] > 0)
                        best = max(best, dp[i - 1][j] + dungeon[i][j]);
                    if (j > 0 && dp[i][j - 1] > 0)
                        best = max(best, dp[i][j - 1] + dungeon[i][j]);
                    dp[i][j] = best;
                }
            }
            return dp[n - 1][m - 1] > 0;
        };

        int l = 1, r = INT_MAX / 2;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (canReach(mid))
                r = mid - 1;
            else
                l = mid + 1;
        }
        return l;
    }
};
