#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        // 翻转一个数字, 方便从低索引向高索引便利, 同时方便push_back新的carry
        reverse(digits.begin(), digits.end());

        // 一开始carry设为1, 表示+1
        int carry = 1;
        int n = digits.size();
        for (int i = 0; i < n || carry /* 如果还有原本的数或者carry */; i++) {
            if (i >= n) digits.push_back(carry);
            else digits[i] += carry;
            // 更新carry
            carry = digits[i] / 10;
            // 更新digits
            digits[i] = digits[i] % 10;
        }

        // 再次反转回来
        reverse(digits.begin(), digits.end());
        return digits;
    }
};

