#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minimumPairRemoval(vector<int>& nums) {
        if (nums.size() <= 1) return 0;

        int ans = 0;
        vector<int> nums1(nums.begin(), nums.end());
        vector<int> nums2;

        while (!check(nums1)) {
            int min_ = INT_MAX;
            int min_i = -1;
            for (int i = 0; i < nums1.size() - 1; i++) {
                if (nums1[i] + nums1[i + 1] <= min_)  { // 更新当前最小值和其坐标
                    min_ = nums1[i] + nums1[i + 1];
                    min_i = i;
                }
            }
            ans++;
            // 循环结束删除两个元素, 添加一个元素
            nums1.erase(nums1.begin() + min_i, nums1.begin() + min_i + 1);
            nums1.insert(nums1.begin() + min_i, min_);
        }

        return ans;
    }

    // 检查是否非递减
    bool check(vector<int> &nums) {
        if (nums.size() == 1) return true;
        for (int i = 0; i < nums.size() - 1; i++) {
            if (nums[i] > nums[i + 1]) {
                return false;
            }
        }
        return true;
    }
};
