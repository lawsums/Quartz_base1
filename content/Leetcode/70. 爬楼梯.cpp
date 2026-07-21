#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int climbStairs(int n) {
        // 这样求得是最少需要多少步能到达这个阶梯
        // auto dp = vector<int>(n + 1, INT_MAX);
        // dp[0] = 0; // 一开始0处初始化为0, 其他地方初始化为正无穷
        // for (int i = 1; i <= n; i++)
        // {
        //     if (i > 1) dp[i] = min(dp[i - 1] + 1, dp[i]);
        //     if (i > 2) dp[i] = min(dp[i - 2] + 1, dp[i]); 
        // }
        // return dp[n];
            
        auto dp = vector<int>(n + 1, 0);
        dp[0] = 1; // 一开始0处初始化为1, 其他地方初始化为0
        for (int i = 1; i <= n; i++)
        {
            if (i >= 1) dp[i] += dp[i - 1];
            if (i >= 2) dp[i] += dp[i - 2];
        }
        return dp[n];

    }
};

