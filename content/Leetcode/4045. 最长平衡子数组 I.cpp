#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestBalanced(vector<int>& nums) {
        int n = nums.size(), ans = 0;
        for(int l = 0; l < n; l++) {
            unordered_set<int> odd, even;
            for(int r = l; r < n; r++) {
                if(nums[r]&1) odd.insert(nums[r]);
                else even.insert(nums[r]);
                if(odd.size() == even.size()) ans = max(ans, r - l + 1);
            }
        }
        return ans;
    }
};
