#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int coinChange(vector<int>& coins, int n) {
        vector<int> dp(n + 1,  n + 1); // 不可能是n + 1, 因为coin最低也是1, 达成n最多需要n, 所以如果结果是n + 1返回-1
        dp[0] = 0;
        for (const auto &c : coins) {
            for (int i = 0; i < n + 1; i++) {
                if (i >= c) {
                    dp[i] = min(dp[i], 1 + dp[i - c]);
                }
            }
        }
        return dp.back() == n + 1 ? -1 : dp.back();
    }
};

