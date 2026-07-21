#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        // 寻找每个数左右比它小的坐标       
        int n = heights.size();
        auto ll = vector<int>(n, -1);
        auto rl = vector<int>(n, n);
    
        vector<int> stk;
        for (int i = 0; i < n; i++) {
            while (!stk.empty() && /* 我们需要维持一个单调递增, 如果不单调递增的话, 那么就可以把所有栈中的元素'右最近更小'更新 */ heights[i] < heights[stk.back()]) {
                rl[stk.back()] = i; // 更新
                stk.pop_back(); // 弹出元素防止继续更新
            }
            stk.push_back(i);
        } // 如果没有更新那么就默认n

        stk.clear();
        for (int i = n - 1; i >= 0; i--) {
            while (!stk.empty() && /* 我们需要维持一个单调递增, 如果不单调递增的话, 那么就可以把所有栈中的元素'左最近更小'更新 */ heights[i] < heights[stk.back()]) {
                ll[stk.back()] = i; // 更新
                stk.pop_back(); // 弹出元素防止继续更新
            }
            stk.push_back(i);
        } // 如果没有更新那么就默认-1

        // display(rl);
        // display(ll);
        int ans = 0;
        for (int i = 0; i < n; i++) {
            ans = max(ans, heights[i] * (rl[i] - ll[i] - 1));
        }
        return ans;
    }

    void display(vector<int> &arr) {
        for (int i = 0; i < arr.size(); i++) {
            printf("%d, ", arr[i]);
        }
        cout << endl;
    }
};

