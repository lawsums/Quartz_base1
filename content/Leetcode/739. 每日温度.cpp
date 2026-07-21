// #include <bits/stdc++.h>
// using namespace std;
//
// class Solution {
// public:
//     vector<int> dailyTemperatures(vector<int>& temperatures) {
//         int n = temperatures.size();
//         vector<int> ans(n, 0);      
//         vector<int> stk;
//         // 构建单调栈
//         for (int i = 0; i < n; i++) { 
//             while (!stk.empty() && temperatures[i] < /* TODO 可能是<= */ temperatures[stk.back()]) {
//                 // 弹出元素
//                 stk.pop_back();
//             }
//             // 加入栈中元素
//             if (!stk.empty()) ans[stk.back()] = i - stk.back();
//             stk.push_back(i);
//         }
//
//         return ans;
//     }
// }

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int n = temperatures.size();
        vector<int> ans(n, 0);      
        stack<int> stk; // 使用 stack 更直观

        for (int i = 0; i < n; i++) { 
            // 当当前温度大于栈顶温度时，说明找到了栈顶元素的下一个更高温度
            while (!stk.empty() && temperatures[i] > temperatures[stk.top()]) {
                int idx = stk.top();
                stk.pop();
                ans[idx] = i - idx; // 结算被弹出的元素
            }
            stk.push(i); // 将当前元素压入栈
        }

        return ans;
    }
};;


