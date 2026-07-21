#include <bits/stdc++.h>
using namespace std;


// Definition for a binary tree node.
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    int maxLevelSum(TreeNode* root) {
        queue<TreeNode*> q; // 设置一个树节点队列
        q.push(root);

        vector<int> sums{0};
        // int idx = 1;
        while (!q.empty()) {
            int size_ = q.size();
            int sum_ = 0;
            for (int i = 0; i < size_; i++) {
                auto cur = q.front(); q.pop(); // 取出当前元素
                sum_ += cur->val;
                // 加入左右节点
                if (cur->left) q.push(cur->left);
                if (cur->right) q.push(cur->right);
            } 
            sums.push_back(sum_);
        }
        return max_element(sums.begin() + 1, sums.end()) - sums.begin();

    }
};
