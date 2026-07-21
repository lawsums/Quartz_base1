#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int strStr(string haystack, string needle) {
        // return haystack.find(needle);
        return kmp(haystack, needle);
    }

    int kmp(string &s1, string &s2) {
        int n = s1.size(), m = s2.size();
        int x = 0, y = 0;
        vector<int> next = nextArray(s2);
        while (x < n && y < m) {
            if (s1[x] == s2[y]) {
                x++;
                y++;
            } else if (y == 0) { // 如果y等于零的话, 就让x++, 因为next[0] = -1, s2[-1]不存在且越界
                x++;
            } else { // 否则通过next数组向前移动y, 也就相当于向后移动x
                y = next[y];
            }
        }

        return y == m ? x - y : -1;
    }

    vector<int> nextArray(string &s2) {
        if (s2.size() == 1) return { -1 };

        int m = s2.size();
        vector<int> next(m, 0); 
        next[0] = -1;
        next[1] = 0;

        int i = 2, cn = 0;
        while (i < m) {
            if (s2[i - 1] == s2[cn]) { // 如果匹配正常的话, 就直接移动两端
                next[i++] = ++cn;
            } else if (cn > 0) { // 跳跃
                cn = next[cn];
            } else { // 如果cn == 0的话, 此时next[cn] = -1, 不能再跳跃了, 直接令next[i]等于零, i++
                next[i++] = 0;
            }
        }
        return next;
    }
};
