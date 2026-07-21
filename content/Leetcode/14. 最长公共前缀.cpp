#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        if (strs.size() == 1) return strs[0];
        sort(strs.begin(), strs.end(), [](string &a, string &b) {
                return a.size() < b.size();  // 按照长度排序
                });

        int i = 0;
        for (; i < strs[0].size(); i++) {
            for (int j = 1; j < strs.size(); j++) {
                if (strs[0][i] != strs[j][i]) {
                    return strs[0].substr(0, i);
                }
            }
        }   

        return strs[0];
    }
};

