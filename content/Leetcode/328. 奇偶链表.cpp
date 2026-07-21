#include <bits/stdc++.h>
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
    ListNode* oddEvenList(ListNode* head) {
        // 记录奇数和偶数节点        
        vector<vector<ListNode*>> even_odd(2, vector<ListNode*>{}); // 申请两个vec

        auto cur = head;
        size_t id = 0;
        // printf("1\n");
        while (cur) {
            // printf("cur->val = %d\n", cur->val);
            even_odd[id].push_back(cur);
            id = 1 - id; // 切换奇偶性
            cur = cur->next; // 下一个节点
        }

        // 先加入偶数节点, 然后奇数节点
        // Q: 会不会由于之前的节点前后联系影响安排?
        // A: 前面的都不会, 因为next都被修改了, 但是最后一个节点会
        ListNode* dummy = new ListNode(0);
        cur = dummy;
        // printf("2\n");
        for (int i = 0; i < 2; i++) {
            for (auto &node : even_odd[i]) {
                cur->next = node;
                cur = cur->next;
            } 
        }
        cur->next = nullptr; // 将最后一个节点的next置为nullptr

        // printf("3\n");
        return dummy->next;
    }
};

