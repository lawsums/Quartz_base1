#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool check(vector<int>& nums) {
        int n = nums.size();

        int inc = 1;
        int ans = 1;
        for (int i = 1; i < 2 * n; ++i) {
            if (nums[i % n] > nums[(i - 1) % n]) {
                inc++; 
                ans = max(ans, inc);
            } else {
                // 从自己开始重新递增
                inc = 1;
            }
        }

        return ans >= n;
    }
};
