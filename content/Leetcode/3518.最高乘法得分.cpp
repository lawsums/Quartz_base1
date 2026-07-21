// 方法一：记忆化 DFS（2D DP，通用解法，m = len(a), n = len(b)）
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long maxScore(vector<int>& a, vector<int>& b) {
        int m = a.size(), n = b.size();
        vector<vector<long long>> memo(m + 1, vector<long long>(n + 1, LLONG_MIN));
        auto dfs = [&](auto&& dfs, int i, int j) -> long long {
            if (i == 0) return 0;
            if (i > j) return LLONG_MIN / 2; // b 的元素不够匹配 a
            if (memo[i][j] != LLONG_MIN) return memo[i][j];
            if (i == j) {
                // 必须选 b[j-1]，否则凑不齐
                return memo[i][j] = 1LL * a[i - 1] * b[j - 1] + dfs(dfs, i - 1, j - 1);
            }
            // j > i: 可选可不选
            return memo[i][j] = max(
                1LL * a[i - 1] * b[j - 1] + dfs(dfs, i - 1, j - 1), // 选
                dfs(dfs, i, j - 1)                                     // 跳过
            );
        };
        return dfs(dfs, m, n);
    }
};


// 方法二：1D DP（利用 len(a) == 4，O(n) 时间，O(1) 空间）
class Solution {
public:
    long long maxScore(vector<int>& a, vector<int>& b) {
        // dp[i] = 匹配 a[0..i] 的最大得分
        vector<long long> dp(4, LLONG_MIN / 2);
        for (int num : b) {
            for (int i = 3; i >= 0; --i) {          // 倒序防重复使用
                dp[i] = max(dp[i],
                    (i > 0 ? dp[i - 1] : 0LL) + 1LL * a[i] * num);
            }
        }
        return dp[3];
    }
};
