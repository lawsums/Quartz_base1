
#include <bits/stdc++.h>
using namespace std;    

class Solution {
private:
public:
    string minWindow(string s, string t) {
        int min_len = INT_MAX; // 记录最小窗口的长度
        int start_idx = -1; // 记录最小窗口的起始坐标
        int depts = t.size(); // 总欠债数
        vector<int> cnts(256, 0);

        for (char c : t) {
            cnts[c]++;
        }

        int l = 0;
        for (int r = 0; r < s.size(); r++) {
            char rc = s[r];
            if (cnts[rc] > 0) { // 说明可以还债
                depts--;
            }
            cnts[rc]--; // 不管需不需要还都可以减去这个cnt, 我们通过这个cnt去计算是否需要把欠债再加上

            while (depts == 0) {
                int current_len = r - l + 1;
                if (current_len < min_len) {
                    min_len = current_len;
                    start_idx = l;
                }

                char lc = s[l];
                if (cnts[lc] == 0) { // 这里是重点!!!!!
                    depts++;
                }
                cnts[lc]++;
                l++;
            }
        }

        return start_idx == -1 ? "" : s.substr(start_idx, min_len);
    }
};

#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

class Solution {
private:
    unordered_map<int, bool> isdept; // 记录这个坐标是不是欠过债, 默认初始化为false
public:
    string minWindow(string s, string t) {
        // 欠债模型 
        string ans = ""; // 初始化ans为空字符串, 用来在找不到答案的时候输出
        int depts = t.size();
        auto cnts = vector<int>(256, 0);
        for (const auto &c : t) {
            cnts[c]++; // 插入一个acsii码的计数表中
        }

        for (int l = 0, r = 0; r < s.size(); r++) { // 初始化左边界和右边界

            // 有债就还债
            if (cnts[s[r]] > 0) {
                isdept[r] = true;
                cnts[s[r]]--;
                depts--;
            }

            // 如果没债了就说明可以作为答案候选, 但是直接选择未必是最短答案, 所以要一边更新答案一边缩减左边界
            while (depts == 0) { 
                if (r - l + 1 < ans.size()) {
                    ans = s.substr(l, r - l + 1);
                }
                if (isdept[l]) { // 如果是之前欠的债的话需要加回去
                    cnts[s[l++]]++;
                    depts++;
                }
            }
        }

        return ans;
    }
};
