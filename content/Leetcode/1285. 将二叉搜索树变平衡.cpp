#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void inorder(TreeNode* node, vector<TreeNode*> &nodes) {
        if (!node) return;

        inorder(node->left, nodes);
        nodes.push_back(node);
        inorder(node->right, nodes);
    }

    TreeNode* build(int l, int r, vector<TreeNode*> &nodes) {
        if (l > r) return nullptr;

        int mid = l + (r - l) / 2;

        auto node = new TreeNode(nodes[mid]->val);
        node->left = build(l, mid - 1, nodes);
        node->right = build(mid + 1, r, nodes);

        return node;
    }

    TreeNode* balanceBST(TreeNode* root) {
        // 获取元素数组
        vector<TreeNode*> nodes;
        inorder(root, nodes);

        // 构建这棵树
        return build(0, nodes.size() - 1, nodes);
    }
};

