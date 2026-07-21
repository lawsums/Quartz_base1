#include <stack>
#include <vector>
#include <algorithm>  // 用于 max 函数（部分编译器需显式包含）
using namespace std;  // LeetCode 提交时默认包含，此处为完整性添加

class Solution {
private:
    int n, m;                  // n：矩阵行数，m：矩阵列数
    vector<vector<int>> grid;  // 存储字符矩阵转换后的整数矩阵（'0'→0，'1'→1）

    /**
     * @brief 字符矩阵转整数矩阵
     * @param matrix 输入的二维字符矩阵（仅含 '0' 和 '1'）
     * 作用：将字符形式的 0/1 转为整数，方便后续柱高计算
     */
    void build(vector<vector<char>>& matrix) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                // 字符转整数：'0'的ASCII码 - '0' = 0，'1'的ASCII码 - '0' = 1
                grid[i][j] = matrix[i][j] - '0';
            }
        }
    }

public:
    /**
     * @brief 主函数：求二维二进制矩阵中只含 '1' 的最大矩形面积
     * @param matrix 输入的二维字符矩阵
     * @return 最大矩形的面积
     * 核心思路：将二维矩阵每行转为直方图，求所有直方图的最大面积（二维转一维降维求解）
     */
    int maximalRectangle(vector<vector<char>>& matrix) {
        // 边界处理：若矩阵为空，直接返回 0（避免 matrix[0] 越界）
        if (matrix.empty() || matrix[0].empty()) return 0;
        
        this->n = matrix.size();        // 初始化行数
        this->m = matrix[0].size();     // 初始化列数
        grid.assign(n, vector<int>(m, 0));  // 初始化整数矩阵（n行m列，初始值0）
        build(matrix);                  // 调用build转换字符矩阵

        int max_area = 0;               // 存储全局最大矩形面积
        vector<int> current_height(m, 0);  // 存储当前行对应的直方图柱高数组

        // 遍历每一行，构建该行对应的直方图
        for (int i = 0; i < n; i++) {
            // 遍历每一列，更新当前列的柱高
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == 1) {
                    // 当前位置是1，柱高 = 上一行柱高 + 1（连续1的高度叠加）
                    current_height[j]++;
                } else {
                    // 当前位置是0，柱高重置为0（无法形成连续1，直方图断开）
                    current_height[j] = 0;
                }
            }
            // 计算当前直方图的最大面积，更新全局最大值
            max_area = max(max_area, f1(current_height));
        }

        return max_area;
    }

    /**
     * @brief 辅助函数：求一维直方图的最大矩形面积（单调栈经典解法）
     * @param heights 直方图的柱高数组（输入为current_height）
     * @return 该直方图的最大矩形面积
     * 核心原理：对每个柱体，找到左右两侧第一个更矮的柱体，计算以该柱体为高的最大矩形面积
     */
    int f1(vector<int>& heights) {
        int n = heights.size();  // n = 柱体个数（即矩阵列数m）
        vector<int> left(n, -1);  // left[i]：i左侧第一个比heights[i]小的柱体索引（默认-1，代表无）
        vector<int> right(n, n);  // right[i]：i右侧第一个比heights[i]小的柱体索引（默认n，代表无）
        stack<int> mono_stack;    // 单调栈：存储柱体索引，确保索引对应的柱高严格递增

        // 第一遍遍历：求每个柱体的left[i]（左侧第一个更矮柱体）和right[i]（右侧第一个更矮柱体）
        for (int i = 0; i < n; ++i) {
            // 栈非空，且当前柱高 < 栈顶索引的柱高：栈顶元素的右侧第一个更矮柱体就是i
            while (!mono_stack.empty() && heights[mono_stack.top()] >= heights[i]) {
                right[mono_stack.top()] = i;  // 记录栈顶元素的right[i]
                mono_stack.pop();             // 栈顶元素已找到right，弹出（后续无需再处理）
            }
            // 此时栈顶是i左侧第一个更矮柱体（栈空则为-1）
            left[i] = mono_stack.empty() ? -1 : mono_stack.top();
            mono_stack.push(i);  // 当前索引入栈，维护栈的递增性质
        }

        int max_rect = 0;  // 存储当前直方图的最大矩形面积
        // 第二遍遍历：计算每个柱体对应的最大矩形面积
        for (int i = 0; i < n; ++i) {
            // 宽度 = 右侧第一个更矮柱体索引 - 左侧第一个更矮柱体索引 - 1
            int width = right[i] - left[i] - 1;
            // 面积 = 柱高 * 宽度（以当前柱体为高的最大矩形）
            int area = heights[i] * width;
            max_rect = max(max_rect, area);  // 更新最大值
        }

        return max_rect;
    }
};
