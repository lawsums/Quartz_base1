#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int magicalString(int n) {
        string s = "1";
        int ans = 1;
        bool one = true;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < s[i] - '0' - 1; j++) {
                s.push_back(one ? '1' : '2');
                if (one) ans++;
                if (s.size() >= n) return ans;
            }
            one = !one;
            s.push_back(one ? '1' : '2');
            if (one) ans++;
            if (s.size() >= n) return ans;
        }
            
        return ans;
    }
};


