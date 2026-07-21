#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<int> dp(n, 1); // 第一行都是只有一条路
        // 滚动数组
        for (int i = 1/*从第二行开始*/; i < m; i++) {
            for (int j = 1/*j == 0的结果是不变的*/; j < n; j++) {
                dp[j]  = dp[j] + dp[j - 1];
            }
        }
        return dp.back();
    }
};



