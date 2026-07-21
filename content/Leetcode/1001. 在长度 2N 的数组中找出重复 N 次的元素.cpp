#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

class Solution {
public:
    int repeatedNTimes(vector<int>& nums) {
        unordered_map<int, int> cnt;
        for (const auto& num : nums) {
            if (cnt.count(num)) return num;
            cnt[num]++;
        }
        return -1;
    }
};
