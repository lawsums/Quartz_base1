#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> finalPrices(vector<int>& prices) {
        // 使用单调栈, 栈中存储下标
        int n = prices.size(); 
        vector<int> stk;
        // 一开始将所有的右边界设为不存在
        vector<int> rs(n, n);
        for (int i = n - 1; i >= 0; i--) {
            // 构建单调递增的栈 
            while (!stk.empty() && prices[i] < prices[stk.back()]) {   /* 用成if了, 但是应该使用while!!!!! */
                stk.pop_back();
            } // 如果不单调递增 prices[i] < prices[i + 1] 就需要pop
            rs[i] = (!stk.empty() ?  stk.back() : n);
            stk.push_back(i);
        }

        vector<int> ans = prices;
        for (int i = 0; i < n; i++) {
            if (rs[i] != n) {
                // printf("%d - %d\n", ans[i], prices[rs[i]]);
                ans[i] -= prices[rs[i]];
            }
        }
        return ans;

    }
};

