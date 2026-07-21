#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minCut(string s) {
        // 将字符串 s 的每个子串是否为回文串预先计算出来
        int n = s.size();
        vector<vector<int>> g(n, vector<int>(n, true));

        // g(i, j) = {  True, i >= j -> 如果 i == j 直接返回true, 单个字符不用判断
        //              g(i + 1, j - 1) -> 之前是否是回文串 && s[i] == s[j] -> 新加入的两边是否相同, i < j
        for (int i = n - 1; i >= 0; --i) { // i倒着来遍历
            for (int j = i + 1; j < n; ++j) {
                g[i][j] = (s[i] == s[j] && g[i + 1][j - 1]);
            }
        }

        if (g[0][n - 1]) return 0; // 如果整个字符串就是回文的, 直接返回true


        // 我们通过f(i) -> 前i个字符中最少分割几次能够分成全是回文串
        vector<int> f(n + 1, INT_MAX / 2);

        // 初始化所有的f, 如果它们本身就是回文的话直接是0
        for (int i = 1; i < n + 1; i++) { 
            if (g[0][i - 1]) {
                f[i] = 0;
            }
        }

        // f(i) = min({f(j) + 1}) if g[j + 1][i], 0 <= j < i
        // f(1) = min(self, f(0) + 1) if g[0][0] == true
        // f(2) = f(1) + 1 if g[1][1]
        // f(2) = min(self, f(0)) if g[0][1]
        for (int i = 1; i < n + 1; i++) {
            for (int j = 0; j < i; j++) {
                if (g[j][i - 1]) { // 回文字符串
                    f[i] = min(f[i], f[j] + 1);
                }
            }
        }

        return f[n];
    }
};

