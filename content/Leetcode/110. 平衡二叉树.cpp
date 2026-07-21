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

struct Info {
    int height;
    bool is_balanced;

    Info (int height, bool is_balanced) : height(height), is_balanced(is_balanced) {}
};

class Solution {
public:
    bool isBalanced(TreeNode* root) {
        return f1(root);
    }

    // 复杂版
    bool f1(TreeNode* root) {
        // 判断是否每个左子树和右子树都是平衡的
        return handler(root).is_balanced;
    }

    // 简洁版
    bool f2(TreeNode* root) {
        return b_d(root).first;
    }

private:
    pair<bool, int> b_d(TreeNode* r){
        if(!r) return {true,0};
        auto [lb,ld] = b_d(r->left);
        auto [rb,rd] = b_d(r->right);
        return {lb && rb && abs(ld-rd)<=1, 1+max(ld,rd)};
    }

    Info handler(TreeNode* root) {
        if (!root) return Info(0, true);

        auto infol = handler(root->left);
        auto infor = handler(root->right);
        int height = 1 + max(infol.height, infor.height);
        bool is_balanced = abs(infol.height - infor.height) <= 1 && infol.is_balanced && infor.is_balanced;
        return Info(height, is_balanced);
    }

};
