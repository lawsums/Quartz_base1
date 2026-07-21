#include <bits/stdc++.h>
using namespace std;

class MinStack {
private:
    vector<int> stk;
    vector<int> min_stack;

public:
    MinStack() {
        
    }
    
    void push(int val) {
        if (min_stack.empty() || val <= min_stack.back()) { // 因为如果有多个同等数值的最小值, 我们都需要记录, 不然的话, pop一个元素之后, 无法确定还有没有这个数值的最小值了
            min_stack.push_back(val);
        }
        stk.push_back(val);
    }
    
    void pop() {
        if (stk.empty()) return; 
        if (min_stack.back() == stk.back()) { // 如果最小值等于堆栈的弹出值那么也弹出
            min_stack.pop_back();
        }
        stk.pop_back();
        
    }
    
    int top() {
        if (stk.empty()) return -1; // 不会有这个情况
        return stk.back();
    }
    
    int getMin() {
        
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */
