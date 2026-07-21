#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
public:
    string minWindow(string s, string t) {
        int min_len = INT_MAX;// 记录最小窗口的长度
        int start_idx = -1;   // 记录最小窗口的起始坐标
        int depts = t.size(); // 总欠债数
        vector<int> cnts(256, 0);

        for (char c: t) {
            cnts[c]++;
        }

        int l = 0;
        for (int r = 0; r < s.size(); r++) {
            char rc = s[r];
            if (cnts[rc] > 0) {// 说明可以还债
                depts--;
            }
            cnts[rc]--;// 不管需不需要还都可以减去这个cnt, 我们通过这个cnt去计算是否需要把欠债再加上

            while (depts == 0) {
                int current_len = r - l + 1;
                if (current_len < min_len) {
                    min_len = current_len;
                    start_idx = l;
                }

                char lc = s[l];
                if (cnts[lc] == 0) {// 这里是重点!!!!!
                    depts++;
                }
                cnts[lc]++;
                l++;
            }
        }

        return start_idx == -1 ? "" : s.substr(start_idx, min_len);
    }
};