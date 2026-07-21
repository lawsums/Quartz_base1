
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.size();
        if (n == 0) return "";

        int len_ = 1;
        int id_ = 0;
        for (int i = 0; i < n - 1; i++) {
            for (int j = n - 1; j > i + len_ - 1; j--) {
                if (check(s, i, j)) {
                    if (j - i + 1 > len_) {
                        len_ = j - i + 1;
                        id_ = i;
                    }
                    break;
                }
            }
        }

        return s.substr(id_, len_);
    } 

    bool check(string& s, int l, int r) {
        while (l < r) {
            if (s[l++] != s[r--]) {
                return false;
            }
        }
        return true;
    }
};
