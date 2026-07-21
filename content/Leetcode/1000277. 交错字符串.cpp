#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    vector<vector<int>> dp;  // dp[i][j] 表示s1[0..i-1]和s2[0..j-1]能否组成s3[0..i+j-1]
    string s1, s2, s3;
    int n, m, len3;
public:
    bool isInterleave(string s1, string s2, string s3) {
        initial(s1, s2, s3);
        // return f1();
        return f2();
    }

    // 记忆化搜索
    bool f1() {
        // 先判断长度是否匹配，不匹配直接返回false
        if (n + m != len3) {
            return false;
        }

        // 初始化dp数组：(n+1)x(m+1)，值为-1（未计算）、0（false）、1（true）
        dp.assign(n + 1, vector<int>(m + 1, -1));

        // 深度优先搜索（带记忆化）
        return dfs(n - 1, m - 1, len3 - 1);
    }

    // 动态规划
    bool f2() {
        if (n + m != len3) return false;

        dp.assign(n + 1, vector<int>(m + 1, 0));
        dp[0][0] = 1;

        for (int i = 1; i < n + 1; i++) {
            if (s1[i - 1] == s3[i - 1]) {
                dp[i][0] = dp[i - 1][0];
            }
        }
        for (int j = 1; j < m + 1; j++) {
            if (s2[j - 1] == s3[j - 1]) {
                dp[0][j] = dp[0][j - 1];
            }
        }

        for (int i = 1; i < n + 1; i++) {
            for (int j = 1; j < m + 1; j++) {
                if (s1[i - 1] == s3[i + j - 1]) {
                    dp[i][j] |= dp[i - 1][j];
                }
                if (s2[j - 1] == s3[i + j - 1]) {
                    dp[i][j] |= dp[i][j - 1];
                }
            }
        }

        return dp[n][m] == 1;
    }

private:
    void initial(string s1, string s2, string s3) {
        this->s1 = s1;
        this->s2 = s2;
        this->s3 = s3;
        n = s1.size();
        m = s2.size();
        len3 = s3.size();
    }

    bool dfs(int i, int j, int step) { 
        // 递归终止条件：所有字符都匹配完成
        if (step == -1) {
            return i == -1 && j == -1;
        } 

        // 记忆化：用i+1/j+1规避负数索引，访问dp[i+1][j+1]
        if (dp[i+1][j+1] != -1) {
            return dp[i+1][j+1] == 1;
        }

        bool result = false;
        // 分支1：匹配s1的当前字符
        if (i >= 0 && s3[step] == s1[i]) {
            result = dfs(i - 1, j, step - 1); 
        }

        // 分支2：如果s1匹配失败，尝试s2
        if (!result && j >= 0 && s3[step] == s2[j]) {
            result = dfs(i, j - 1, step - 1);
        }

        // 缓存结果：1表示true，0表示false
        dp[i+1][j+1] = result ? 1 : 0;
        return result;

    }

};


