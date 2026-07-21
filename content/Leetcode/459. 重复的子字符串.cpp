#include <bits/stdc++.h>
// #include <string>
using namespace std;


class Solution {
public:
    bool repeatedSubstringPattern(string s) {
        return f2(s);
    }

    // 我原先的方法, 逻辑上很蠢, 因为我想不到一下退出两层循环的方式
    bool f1(string s) {
        // 一个字符串多次构成, 那么肯定就是从开头取一个字符串, 然后多次匹配
        int n = s.size();
        for (int l = 1; l < s.size(); l++) {
            bool ans = true;
            if (n % l == 0) { // 如果能整除说明有可能是的
                for (int i = 0; i < l; i++) { // i表示一个外层子串偏移量, 如果这个子串理论上所有位置都一样那么就是对的
                    int c = s[i];
                    for (int j = i + l; j < n; j += l) {
                        if (s[j] != c) { 
                            ans = false;
                            break;
                        }
                    }
                    if (!ans) break;
                }
                if (ans) return true; 
            }
        }

        return false;
    }

    // 方法二, 通过子函数check直接退出, 不错
    bool f2(string s) {
        int n = s.size();
        // 子串至少重复2次，枚举上限直接缩到n/2，基础优化
        for (int l = 1; l <= n / 2; l++) {
            if (n % l == 0) { // 仅整除时验证，无意义循环直接跳过
                // 抽成函数，验证通过直接return true，无需多层break
                if (check(s, l)) {
                    return true;
                }
            }
        }
        return false;
    }

private:
    // 独立验证函数：判断s是否能由长度为l的子串重复构成
    bool check(const string& s, int l) {
        int n = s.size();
        // 按偏移量验证：第i位的所有步长l字符必须相同
        for (int i = 0; i < l; i++) {
            char c = s[i]; // 基准字符（子串的第i位）
            // 从i+l开始，步长l遍历，只要有一个不同直接返回false
            for (int j = i + l; j < n; j += l) {
                if (s[j] != c) {
                    return false; // 一旦不匹配，直接终止验证
                }
            }
        }
        return true; // 所有偏移位都匹配，验证通过
    }
};
