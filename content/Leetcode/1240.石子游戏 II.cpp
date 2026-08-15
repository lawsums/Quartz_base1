#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int stoneGameII(vector<int>& piles) {
        int n = piles.size();

        // 后缀和，方便 O(1) 计算 piles[i..n-1] 的总和
        vector<int> suffix_sum(n + 1, 0);
        for (int i = n - 1; i >= 0; --i) {
            suffix_sum[i] = suffix_sum[i + 1] + piles[i];
        }

        // memo[i][m] 缓存已计算状态
        vector<vector<int>> memo(n, vector<int>(n + 1, -1));

        // 递归函数：当前玩家从位置 i 开始，M = m，能获得的最大石子数
        function<int(int, int)> dp = [&](int i, int m) -> int {
            if (i >= n) return 0;
            if (memo[i][m] != -1) return memo[i][m];

            // 如果剩余堆数 <= 2M，当前玩家可以全部拿走
            if (i + 2 * m >= n) {
                return memo[i][m] = suffix_sum[i];
            }

            // 枚举拿 X 堆，找对手能拿的最小值
            int min_opponent = INT_MAX;
            for (int x = 1; x <= 2 * m; ++x) {
                int opponent = dp(i + x, max(m, x));
                min_opponent = min(min_opponent, opponent);
            }

            // 当前玩家最大 = 总剩余 - 对手最小
            return memo[i][m] = suffix_sum[i] - min_opponent;
        };

        return dp(0, 1);
    }
};
