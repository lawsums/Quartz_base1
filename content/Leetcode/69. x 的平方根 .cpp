#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int mySqrt(int x) {
        // 二分搜索
        // f(left - 1) <= x
        // f(right + 1) > x
        int left = 0, right = x;
        while (left <= right) {
            long long mid = left + (right - left) / 2;
            if (mid * mid > x) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        return right; // 最后一个 i * i <= x的数
    }
};

