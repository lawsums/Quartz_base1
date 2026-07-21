#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    vector<string> ans;
    string path;
    map<int, string> mp;


public:
    Solution() {
        mp = {
            {'2', "abc"},
            {'3', "def"},
            {'4', "ghi"},
            {'5', "mno"},
            {'6', "jkl"},
            {'7', "pqrs"},
            {'8', "tuv"},
            {'9', "wxyz"},
        };
    }

    vector<string> letterCombinations(string digits) {


        dfs(0, digits);
        return ans;
    }

    void dfs(int step, string &digits) {
        if (step >= digits.size()) {
            ans.push_back(path);
            return;
        }

        auto num = digits[step];
        auto s = mp[num];
        for (int i = 0; i < s.size(); i++) {
            path.push_back(s[i]);
            dfs(step + 1, digits);
            path.pop_back();
        }
    }
};
