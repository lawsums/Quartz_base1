#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int projectionArea(vector<vector<int>>& grid) {
        int n = grid.size(), m = grid[0].size();
        int z_area = countNonZero(grid); // 统计有多少个非零元素
        int x_area = 0, y_area = 0;
        for (int i = 0; i < n; i++) {
            int max_ = 0;
            for (int j = 0; j < m; j++) {
                max_ = max(max_, grid[i][j]); // 获取每一行最大的元素
            }
            y_area += max_;
        }
        for (int j = 0; j < m; j++) {
            int max_ = 0;
            for (int i = 0; i < n; i++) {
                max_ = max(max_, grid[i][j]); // 获取每一列最大的元素
            }
            x_area += max_;
        }
        return x_area + y_area + z_area;
    }

    template <typename T>
    int countNonZero(const std::vector<std::vector<T>>& mat) {
        int count = 0;
        for (const auto& row : mat) {
            // count_if：统计区间内满足条件的元素数量
            // 第一个参数：区间起始迭代器，第二个：区间结束迭代器，第三个：判断谓词（非零）
            count += std::count_if(row.begin(), row.end(), [](T val) { return val != 0; });
        }
        return count;
    }
};

