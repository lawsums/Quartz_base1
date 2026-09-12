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
    int amountOfTime(TreeNode* root, int start) {
        unordered_map<int, TreeNode*> node_map;      // 值 -> 节点
        unordered_map<TreeNode*, TreeNode*> parent;  // 节点 -> 父节点

        // DFS 建图：记录父节点 + 值到节点的映射
        function<void(TreeNode*, TreeNode*)> dfs = [&](TreeNode* node, TreeNode* father) {
            if (!node) return;
            parent[node] = father;
            node_map[node->val] = node;
            dfs(node->left, node);
            dfs(node->right, node);
        };
        dfs(root, nullptr);

        // BFS 分层扩散
        queue<int> q;
        unordered_set<int> visit;
        q.push(start);
        visit.insert(start);
        int ans = 0;

        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; ++i) {
                int val = q.front();
                q.pop();
                TreeNode* cur = node_map[val];

                // 父节点
                TreeNode* father = parent[cur];
                if (father && !visit.count(father->val)) {
                    visit.insert(father->val);
                    q.push(father->val);
                }
                // 左子节点
                if (cur->left && !visit.count(cur->left->val)) {
                    visit.insert(cur->left->val);
                    q.push(cur->left->val);
                }
                // 右子节点
                if (cur->right && !visit.count(cur->right->val)) {
                    visit.insert(cur->right->val);
                    q.push(cur->right->val);
                }
            }
            ++ans;
        }

        return ans - 1;  // 最后一轮扩散后没有新感染，多计了一次
    }
};
