#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxSideLength(vector<vector<int>>& grid, int threshold) {
        return f2(grid, threshold);
    }

    // 普通前缀和
    int f1(vector<vector<int>>& grid, int threshold) {
        int n = grid.size(), m = grid[0].size();
        int ans = 0;
        auto pre = vector<vector<int>>(n, vector<int>(m, 0));

        // 构造二维前缀和
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                pre[i][j] = grid[i][j] + get(pre, i - 1, j) + get(pre, i, j - 1) - get(pre, i - 1, j - 1);
            }
        }

        // 对于每个位置进行判断是否有更大的正方形
        for (int i = 0; i < n; i++) { 
            for (int j = 0; j < m; j++) {
                for (int k = min(i, j) + 1; k > ans; k--) {
                    if (pre[i][j] - get(pre, i - k, j) - get(pre, i, j - k) + get(pre, i - k, j - k) <= threshold) {
                        ans = k;
                    }
                }
            }
        }

        return ans; // 返回边长而不是面积
    }

    // 二分搜索法
    int f2(vector<vector<int>>& grid, int threshold) {
        int n = grid.size(), m = grid[0].size();
        int ans = 0;
        vector<vector<int>> pre(n + 1, vector<int>(m + 1, 0)); // 前缀和数组多开1行1列，避免边界判断

        // 构造二维前缀和（更简洁的写法，无需get函数）
        for (int i = 1; i <= n; i++) {
            int row_sum = 0;
            for (int j = 1; j <= m; j++) {
                row_sum += grid[i-1][j-1];
                pre[i][j] = pre[i-1][j] + row_sum;
            }
        }

        // 二分查找最大边长：全局范围是 [0, max_len]，max_len是网格能容纳的最大正方形边长
        int max_possible_len = min(n, m);
        int left = 0, right = max_possible_len;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (hasValidSquare(pre, n, m, mid, threshold)) {
                ans = mid;       // 找到合法边长，尝试更大的
                left = mid + 1;
            } else {
                right = mid - 1; // 边长太大，尝试更小的
            }
        }

        return ans;
    }

private:
    int get(vector<vector<int>> &grid, int i, int j) {
        if (i < 0 || j < 0) return 0;
        return grid[i][j];
    }

    // 检查是否存在边长为k的正方形，其和 ≤ threshold
    bool hasValidSquare(vector<vector<int>>& pre, int n, int m, int k, int threshold) {
        if (k == 0) return true; // 边长为0必然合法
        // 遍历所有可能的k*k正方形（右下角顶点为(i,j)）
        for (int i = k; i <= n; i++) {
            for (int j = k; j <= m; j++) {
                // 计算正方形的和：前缀和公式（避免边界判断，因为pre多开了1行1列）
                int sum = pre[i][j] - pre[i - k][j] - pre[i][j - k] + pre[i - k][j - k];
                if (sum <= threshold) {
                    return true;
                }
            }
        }
        return false;
    }
};

