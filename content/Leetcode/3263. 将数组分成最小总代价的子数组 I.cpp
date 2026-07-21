#include <bits/stdc++.h>
#include <numeric>
using namespace std;

class Solution {
public:
    int minimumCost(vector<int>& nums) {
        // 选第一个然后剩下两个选两个小的 
        sort(nums.begin() + 1, nums.end());
        // 注意accumulate也是左闭右开区间
        return accumulate(nums.begin(), nums.begin() + 3, 0); 
    }
};

