#include <bits/stdc++.h>
using namespace std;

// 值域树状数组（不离散化）：下标直接使用 rating 的值，大小 = max_rating
class FenwickTree {
    int n;
    vector<int> tree;

public:
    FenwickTree(int n) : n(n), tree(n + 1) {}

    // 下标 i 处增加 val（1-based）
    void update(int i, int val) {
        while (i <= n) {
            tree[i] += val;
            i += i & -i;
        }
    }

    // 前缀和 [1..i]
    int pre(int i) const {
        int res = 0;
        while (i > 0) {
            res += tree[i];
            i &= i - 1;
        }
        return res;
    }

    // 区间和 [l..r]
    int query(int l, int r) const {
        if (r < l) return 0;
        return pre(r) - pre(l - 1);
    }
};

class Solution {
public:
    int numTeams(vector<int>& rating) {
        int n = rating.size();
        int max_rating = *max_element(rating.begin(), rating.end());

        // 后缀树状数组：初始包含所有元素，遍历时边走边减量更新
        FenwickTree left_tree(max_rating), right_tree(max_rating);
        for (int r : rating) right_tree.update(r, 1);

        int ans = 0;
        for (int r : rating) {
            // 当前元素作为中间点 j，先从右侧移除
            right_tree.update(r, -1);

            int left_less     = left_tree.pre(r - 1);        // 左侧比 r 小
            int left_greater  = left_tree.query(r + 1, max_rating); // 左侧比 r 大
            int right_less    = right_tree.pre(r - 1);       // 右侧比 r 小
            int right_greater = right_tree.query(r + 1, max_rating); // 右侧比 r 大

            ans += left_less    * right_greater;  // 递增: i < j < k
            ans += left_greater * right_less;     // 递减: i > j > k

            // 当前元素加入左侧，供后续元素使用（增量更新）
            left_tree.update(r, 1);
        }
        return ans;
    }
};
