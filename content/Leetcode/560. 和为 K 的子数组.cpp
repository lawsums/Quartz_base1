#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        // 前缀和
        unordered_map<int, int> mp; // <前缀和, 个数>
        mp[0]++;
        int ans = 0;
        for (int i = 0, cur = 0; i < nums.size(); i++) {
            cur += nums[i];
            mp[cur]++;
            if (mp.count(cur - k)) {
                ans += mp[cur - k];
            }
        }
        return ans;
    }

    int f1(vector<int>& nums, int k) {
        int ans = 0;
        for (int l = 0, r = 0, cur = 0; r < nums.size(); r++) { // 不断遍历右边界
            cur += nums[r];
            while (cur >= k) { // 如果大于等于k就要缩减左边界了, 同时判断是否是答案之一
                if (cur == k) ans++;
                cur -= nums[l++];
            }
        }

        return ans;
    }
};

// 思考, 如果这道题没有这个限制: -1000 <= nums[i] <= 1000, 而是1 <= nums[i] <= 1000, 是不是就可以使用滑动窗口解题了呢? 如果是0 <= nums[i] <= 1000呢?
