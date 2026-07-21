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
private:
    int ans = 0;

public:
    int diameterOfBinaryTree(TreeNode* root) {
        // 使用handler函数, 为什么要使用handler函数? 因为两个函数返回值不同
        handler(root);
        return ans;
    }

    int handler(TreeNode* root) {
        if (!root) return 0; 
        if (!root->left && !root->right) return 0;

        int left = root->left ? 1 + handler(root->left) : 0;
        int right = root->right ? 1 + handler(root->right) : 0;

        ans = max(ans, left + right);
        return max(left, right);
    }
};
