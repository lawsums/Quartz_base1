#include <bits/stdc++.h>
#include <vector>
using namespace std;

// Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode *insertionSortList(ListNode *head) {
        // 我打算使用栈倒着排序
        vector<ListNode *> stk;
        auto cur = head;
        while (cur) {
            stk.push_back(cur);
            cur = cur->next;
        }

        auto p1 = stk.back();
        for (; !stk.empty(); stk.pop_back()) {
            auto p2 = p1->next;
            if (!p2) continue;
            // p2 != nullptr
            // 如果有p2就比较大小
            while (p1->val < p2->val) {
                if (p2->next)
                    p2 = p2->next;
                else
                    break;
            }
            // p1->val > p2->val || p2->next == nullptr
            
        }
    }
};
