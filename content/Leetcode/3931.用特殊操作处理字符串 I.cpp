#include <bits/stdc++.h>
#include <cctype>
using namespace std;

class Solution {
public:
    string processStr(string s) {
        string result{};

        for (const auto &c: s) {
            if (isalpha(c)) {
                result.push_back(c);
            } else if (c == '*') {
                result.pop_back();
            } else if (c == '#') {
                result += result;
            } else if (c == '%') {
                reverse(result.begin(), result.end());
            }
        }

        return result;
    }
};
