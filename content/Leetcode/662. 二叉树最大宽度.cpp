#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

// typedef unsigned long long ull;
using ull = unsigned long long; // 全新写法!!!

// Definition for a binary tree node.
struct TreeNode {
    ull val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(ull x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(ull x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};


class Solution {
public:
    // 求宽度
    int widthOfBinaryTree(TreeNode* root) {
        if (root == nullptr) return 0;
         return f1(root);
    }

    // 写法一: 通过哈希表+递归实现
    int f1(TreeNode* root) {
        unordered_map<int, ull> max_id; //  记录每一层最小和最大的id
        unordered_map<int, ull> min_id; 
        int h = height(root);
        handler(root, 0, 1, max_id, min_id); // 序号从1开始
        ull ans = 0;
        for (int i = 0; i < h; i++) {
            ans = max(ans, max_id[i] - min_id[i] + 1);
            // prllf("min = %d, max = %d\n", min_id[i], max_id[i]);
        }
        // cout << endl;

        return ans;
    }
    
    // 写法二: BFS写法
    int f2(TreeNode* root) {
        if (!root) return 0;
        
        queue<pair<TreeNode*, ull>> q;
        q.push({root, 1});
        ull maxWidth = 0;
        
        while (!q.empty()) {
            int size = q.size();
            ull left = q.front().second;
            ull right = left;
            
            for (int i = 0; i < size; i++) {
                auto [node, id] = q.front();
                q.pop();
                right = id;
                
                if (node->left) q.push({node->left, 2 * id});
                if (node->right) q.push({node->right, 2 * id + 1});
            }
            
            maxWidth = max(maxWidth, right - left + 1);
        }
        
        return maxWidth;
    }

private:

    int height(TreeNode *root) {
        if (root == nullptr) return 0;
        return 1 + max(height(root->left), height(root->right)); 
    }

    void handler(TreeNode* node, int depth, ull id, unordered_map<int, ull> &max_id, unordered_map<int, ull> &min_id) {
        if (node == nullptr) return;

        // 处理自己这一层
        // max_id[depth] = max(max_id[depth], id); // 初始化没问题 
        if (max_id[depth] == 0 || max_id[depth] < id) max_id[depth] = id;
        if (min_id[depth] == 0 || min_id[depth] > id) min_id[depth] = id;

        handler(node->left, depth + 1, 2 * id, max_id, min_id);
        handler(node->right, depth + 1, 2 * id + 1, max_id, min_id);
    }
};

