#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string addBinary(string a, string b) {
        // 反转两个字符串
        reverse(a.begin(), a.end());
        reverse(b.begin(), b.end());

        // 设置一个carry位
        int carry = 0;
        vector<char> res;
        int n = a.size(), m = b.size();
        for (int i = 0, op1, op2, cur; carry || i < max(n, m); i++) {
            op1 = i < n ? (a[i] - '0') : 0;
            op2 = i < m ? (b[i] - '0') : 0;
            cur = (carry + op1 + op2) % 2;
            carry = (carry + op1 + op2) / 2;
            res.push_back(cur + '0');
        }

        reverse(res.begin(), res.end());
        return string(res.begin(), res.end());
    }
};

int main() {
    string a = "11", b = "1";
    Solution().addBinary(a, b);
}