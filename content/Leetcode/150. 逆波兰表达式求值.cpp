#include <bits/stdc++.h>
#include <cctype>
#include <string>
using namespace std;

// 输入：tokens = ["4","13","5","/","+"]
// 输出：6
// 解释：该算式转化为常见的中缀算术表达式为：(4 + (13 / 5)) = 6

class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        vector<int> num_stk;
        string ops = "+-*/";
        for (int i = 0; i < tokens.size(); i++) {
            if (tokens[i].size() == 1 && ops.find(tokens[i]) != string::npos) { // 说明是符号 
                int op2 = num_stk.back(); num_stk.pop_back();
                int op1 = num_stk.back(); num_stk.pop_back();
                if (tokens[i] == "+") {
                    num_stk.push_back(op1 + op2);
                } else if (tokens[i] == "-") {
                    num_stk.push_back(op1 - op2);
                } else if (tokens[i] == "*") {
                    num_stk.push_back(op1 * op2);
                } else if (tokens[i] == "/") {
                    num_stk.push_back(op1 / op2);
                }
            } else { // 说明是数字
                num_stk.push_back(stoi(tokens[i]));
            }
        }

        return num_stk.back();
        
    }
};
