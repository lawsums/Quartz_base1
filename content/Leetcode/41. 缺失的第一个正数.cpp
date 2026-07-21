#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        return f1(nums);
    }

    // 鸽巢原理 O(n), O(1)
    int f1(vector<int>& nums) {
        int n = nums.size();
        
        // 第一步：将 [1, n] 范围内的数归位到对应的索引位置
        for (int i = 0; i < n; ++i) {
            // 循环交换：直到当前位置的数不在 [1, n] 范围内，或已经归位
            // 注意：必须用 while 而非 if，因为交换过来的数可能也需要归位
            while (nums[i] >= 1 && nums[i] <= n && nums[nums[i] - 1] != nums[i]) {
                // 交换 nums[i] 和 nums[nums[i]-1]，让 nums[i] 归位
                swap(nums[i], nums[nums[i] - 1]);
            }
        }
        
        // 第二步：遍历数组，找第一个“数≠索引+1”的位置
        for (int i = 0; i < n; ++i) {
            if (nums[i] != i + 1) {
                return i + 1;
            }
        }
        
        // 第三步：如果 [1, n] 都在，返回 n+1
        return n + 1;
    }

    // 哈希表 O(n), O(n)
    int f2(vector<int>& nums) {
        int max_ = *max_element(nums.begin(), nums.end());
        if (max_ <= 0) return 1;
        
        unordered_map<int, int> mp;
        for (auto num : nums) {
            mp[num]++;
        }
        
        for (int i = 1; i <= max_; i++) {
            if (!mp.count(i)) return i;
        }

        return max_ + 1;
    }
};

