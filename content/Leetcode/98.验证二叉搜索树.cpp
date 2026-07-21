#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct Info {
    bool is_bst;
    ll min_;
    ll max_;

    Info (bool is_bst, ll min_, ll max_) : 
        is_bst(is_bst), min_(min_), max_(max_) { }
};

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return dfs(root).is_bst;        
    }

    Info dfs(TreeNode* node) {
        if (!node) return Info(true, LLONG_MAX, LLONG_MIN); 
        if (!node->left && !node->right) return Info(true, node->val, node->val); 
        
        auto Infol = dfs(node->left);
        auto Infor = dfs(node->right);

        bool is_bst = Infol.max_ < node->val && node->val < Infor.min_ && Infol.is_bst && Infor.is_bst;
        ll min_ = min({Infol.min_, Infor.min_, 1LL * node->val});
        ll max_ = max({Infol.max_, Infor.max_, 1LL * node->val});

        return Info(is_bst, min_, max_);
    }
};


