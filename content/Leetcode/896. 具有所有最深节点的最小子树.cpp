class Solution {
public:
    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        int max_depth = -1; // 全局最大深度
        TreeNode* ans;

        auto dfs = [&](this auto&& dfs, TreeNode* node, int depth) {
            if (node == nullptr) {
                max_depth = max(max_depth, depth); // 维护全局最大深度
                return depth;
            }

            int left_max_depth = dfs(node->left, depth + 1); // 获取左子树最深叶节点的深度
            int right_max_depth = dfs(node->right, depth + 1); // 获取右子树最深叶节点的深度

            if (left_max_depth == right_max_depth && left_max_depth == max_depth) {
                ans = node; // node 可能是答案
            }

            return max(left_max_depth, right_max_depth); // 当前子树最深叶节点的深度
        };

        dfs(root, 0);
        return ans;
    }
};
