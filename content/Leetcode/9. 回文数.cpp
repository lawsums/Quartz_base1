#include <bits/stdc++.h>
#include <string>
using namespace std;

class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0) return false;

        string s = to_string(x);
        int l = 0, r = s.size() - 1;
        if (s[0] == '-') {
            l++;
        }

        while (l < r) {
            // printf("l = %d, r = %d", l, r);
            if (s[l++] != s[r--]) return false;
        }
        return true;
    }
};

