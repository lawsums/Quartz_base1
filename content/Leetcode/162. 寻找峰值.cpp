#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int n = nums.size();
        // 特殊情况：只有一个元素
        if (n == 1) return 0;
        
        // 二分查找（左闭右闭区间）
        int left = 0, right = n - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            // 检查是否为峰值（需要避免越界）
            bool isPeak = true;
            if (mid > 0 && nums[mid] <= nums[mid - 1]) {
                isPeak = false; // 左侧更大，不是峰值
            }
            if (mid < n - 1 && nums[mid] <= nums[mid + 1]) {
                isPeak = false; // 右侧更大，不是峰值
            }
            if (isPeak) {
                return mid; // 找到峰值
            }
            
            // 不是峰值，向更大的一侧移动
            if (mid < n - 1 && nums[mid] < nums[mid + 1]) {
                // 右侧更大，峰值在右侧
                left = mid + 1;
            } else {
                // 左侧更大或两侧都小，峰值在左侧
                right = mid - 1;
            }
        }
        
        return -1; // 理论上不会走到这里（题目保证存在峰值）
    }
};
