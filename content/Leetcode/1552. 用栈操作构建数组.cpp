#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    vector<string> ans;

public:
    vector<string> buildArray(vector<int>& target, int n) {
        if (target.empty()) {
            return {};
        }

        // 输入一个整数流
        // 如果是 {2 3 5} 5 -> push pop push push push pop push 
        for (int cur = 1; cur <= target.back(); cur++) {
            if (find(target.begin(), target.end(), cur) != target.end()) { // 如果找到了这个元素说明加入push就行, 否则都要加入
                ans.push_back("Push");
            } else {
                ans.push_back("Push");
                ans.push_back("Pop");
            }
        }

        return ans;
    }

};

