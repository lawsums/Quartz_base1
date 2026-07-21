#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        unordered_map<int, int> cnt;
        for (const auto& num : nums) {
            cnt[num]++;
        }

        vector<int> ans;
        for (int i = 1; i <= nums.size(); i++) {
            if (!cnt.count(i)) ans.push_back(i);
        }
        return ans;
    }
};
