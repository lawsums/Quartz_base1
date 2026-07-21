#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<int> smallerNumbersThanCurrent(vector<int>& nums) {
        // 排序
        auto arr = vector<int>(nums.begin(), nums.end());
        sort(nums.begin(), nums.end());

        // 使用哈希表将各个数字的出现次数记录下来
        unordered_map<int, int> cnt;
        int pre = 0;
        for (const auto& num : nums) {
            if (!cnt.count(num)) cnt[num] = pre;
            pre++;
        } 

        auto ans = vector<int>(arr.size(), 0);
        for (int i = 0; i < arr.size(); i++) {
            ans[i] = cnt[arr[i]];
        }
        return ans;
    }
};
