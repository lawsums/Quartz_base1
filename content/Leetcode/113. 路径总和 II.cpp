#include <bits/stdc++.h>
using namespace std;

class Solution {

};


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
	vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
        return f1(root, targetSum);
    }
    
private:
    vector<vector<int>> ans;
    vector<int> path;
    int targetSum;

public:
    vector<vector<int>> f1(TreeNode* root, int targetSum) {
        this->targetSum = targetSum;
        handler(root, 0);
        return ans;
    }

    void handler(TreeNode *node, int sum) {
        if (!node) return;
        sum += node->val;
        // printf("sum = %d\n", sum);
        if (!node->left && !node->right) { // 如果是叶子节点
            if (sum == targetSum) {
                path.push_back(node->val); 
                ans.push_back(path);
                path.pop_back(); 
            }
            return;
        }

        // 传递给下一层节点，sum加上当前节点的值
        path.push_back(node->val); 
        handler(node->left, sum);
        path.pop_back(); 

        path.push_back(node->val); 
        handler(node->right, sum);
        path.pop_back(); 
    }
    
    // 0x3f
    vector<vector<int>> f2(TreeNode* root, int targetSum) {
        vector<vector<int>> ans;
        vector<int> path;

        auto dfs = [&](this auto&& dfs, TreeNode* node, int left) -> void {
            if (node == nullptr) {
                return;
            }
            path.push_back(node->val);
            left -= node->val;
            if (node->left == nullptr && node->right == nullptr && left == 0) {
                ans.push_back(path);
            } else {
                dfs(node->left, left);
                dfs(node->right, left);
            }
            path.pop_back(); // 恢复现场
        };

        dfs(root, targetSum);
        return ans;
    }
};
