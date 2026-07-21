#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> minBitwiseArray(vector<int>& nums) {
        int n = nums.size();
        int m = *max_element(nums.begin(), nums.end());
        unordered_map<int, int> mp; // <一个数，最小的坐标>
        for (int i = 0; i < m; i++) {
            int cur = i | (i + 1);
            if (!mp.count(cur)) mp[cur] = i; // 如果不存在，说明就是最小的坐标
        }

        vector<int> ans(n, -1);
        for (int i = 0; i < n; i++) {
            if (mp.count(nums[i])) ans[i] = mp[nums[i]];
        }
        return ans;
    }

};



