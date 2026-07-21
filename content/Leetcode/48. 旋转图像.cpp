class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();
        // 第一步：主对角线转置（和Go完全一致，j从i+1开始）
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                swap(matrix[i][j], matrix[j][i]); // 直接用std::swap，替代Go的多赋值
            }
        }
        // 第二步：每行水平翻转（循环条件和Go完全一致）
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n / 2; ++j) {
                swap(matrix[i][j], matrix[i][n - 1 - j]); // 同样用swap
            }
        }
    }
};
