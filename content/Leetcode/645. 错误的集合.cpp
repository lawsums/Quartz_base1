#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        // 简单来说就是找到1 - n 之间哪个数字出现了两次和哪个数字一次都没出现
        vector<int> ans;
        sort(nums.begin(), nums.end());
        unordered_map<int, int> cnt;
        for (const auto& num : nums) {
            if (cnt.count(num)) ans.push_back(num);
            cnt[num]++;
        }

        for (int i = 1; i <= nums.size(); i++) {
            if (!cnt.count(i)) ans.push_back(i);
        }
        return ans;
    }
};
