#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // 空间优化版递推 O(n)时间 / O(1)空间
    long long rob(vector<int>& nums, vector<int>& colors) {
        int n = nums.size();

        long long prev2 = nums[0];  // dp[i-2]
        if (n == 1) return prev2;

        long long prev1 = (colors[0] == colors[1])
            ? max((long long)nums[0], (long long)nums[1])
            : (long long)nums[0] + nums[1];  // dp[i-1]
        long long curr = prev1;

        for (int i = 2; i < n; ++i) {
            if (colors[i] == colors[i - 1]) {
                // 同色 → 不能同时拿
                curr = max(prev1, prev2 + nums[i]);
            } else {
                // 不同色 → 可以一起拿
                curr = prev1 + nums[i];
            }
            prev2 = prev1;
            prev1 = curr;
        }
        return prev1;
    }
};
