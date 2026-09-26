#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string evaluate(string s, vector<vector<string>>& knowledge) {
        unordered_map<string, string> mp;
        for (const auto &p : knowledge) {
            string key = p[0], val = p[1];
            mp[key] = val;
        }

        int i = 0;
        string ans = "";
        for (; i < s.size(); i++) {
            // 如果匹配到这个小括号的话，就进入匹配模式
            if (s[i] == '(') {
                i++;
                string tmp = "";
                while (s[i] != ')') {
                    tmp.push_back(s[i++]);
                } // s[i] == ')', tmp = key
                // 如果存在这个键，那么就是掭加到结果里
                if (mp.count(tmp)) {
                    ans += mp[tmp];
                } else {
                    ans.push_back('?');
                }
            } else {
                ans.push_back(s[i]);
            }
        }

        return ans;
    }
};
