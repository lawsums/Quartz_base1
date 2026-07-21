#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxProduct(vector<int>& nums) {
        return f1(nums);
        // return f2(nums);
    }

    // 记录变量法
    int f1(vector<int>& nums) {
        // 记录abs最小的正乘积和负乘积, 但是每次遇到零就要重置
        int min_pos = 1, min_neg = 0; // 0表示不存在
        long long cur = 1;
        int ans = INT_MIN;
        for (const auto &num : nums) {
            cur *= num;
            if (cur > 0) {
                ans = max(ans, (int)(cur / min_pos)); // 最小的正数
            } else if (cur < 0) {
                if (min_neg == 0) {
                    min_neg = cur;
                } else {
                    ans = max(ans, (int)(cur / min_neg));
                }
            } else { // cur == 0
                cur = 1;
                min_pos = 1;
                min_neg = 0;
            }
        }
        return ans == INT_MIN ? *max_element(nums.begin(), nums.end()) : ans;
    }

    // 动态规划
    int f2(vector<int>& nums) { 
        int maxProd = nums[0], minProd = nums[0], ans = nums[0];
        
        for (int i = 1; i < nums.size(); ++i) {
            int num = nums[i];
            // 如果当前数是负数，交换最大和最小乘积
            if (num < 0) {
                swap(maxProd, minProd);
            }
            
            // 更新最大和最小乘积
            maxProd = max(num, maxProd * num);
            minProd = min(num, minProd * num);
            
            // 更新答案
            ans = max(ans, maxProd);
        }
        
        return ans;
    }

};

