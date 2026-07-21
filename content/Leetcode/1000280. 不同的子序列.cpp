#include <bits/stdc++.h>
using namespace std;


class Solution {
public:
    int numDistinct(string s, string t) {
        int n = s.size(), m = t.size();
        // 边界：t为空，s任意子序列都能组成空串，方法数为1；s为空但t非空，方法数为0
        if (m == 0) return 1;
        if (n < m) return 0;
        
        // f[i][j]：s前i个字符组成t前j个字符的方法数
        vector<vector<unsigned int>> f(n + 1, vector<unsigned int>(m + 1, 0));
        // 初始化：t前0个字符（空串），s任意前i个字符都有1种方法（不选任何字符）
        for (int i = 0; i <= n; ++i) f[i][0] = 1;
        
        // 状态转移
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j) {
                // 情况1：不选s的第i个字符（s[i-1]），方法数继承f[i-1][j]
                f[i][j] = f[i-1][j];
                // 情况2：如果s[i-1] == t[j-1]，可以选s[i-1]匹配t[j-1]，累加f[i-1][j-1]
                if (s[i-1] == t[j-1]) {
                    f[i][j] += f[i-1][j-1];
                }
            }
        }
        // 答案：s前n个字符组成t前m个字符的方法数
        return f[n][m];
    }
};
