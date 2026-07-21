#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minimumDeletions(string s) {
        int n = s.size();
        // 预处理：统计每个位置右侧的 'a' 的数量
        vector<int> right_a(n + 1, 0);
        for (int i = n - 1; i >= 0; --i) {
            right_a[i] = right_a[i + 1] + (s[i] == 'a' ? 1 : 0);
        }

        int left_b = 0;  // 统计当前位置左侧的 'b' 的数量
        int min_del = n; // 最小删除次数，初始化为字符串长度（最大可能值）

        // 遍历所有分割点：分割点为 i 表示前 i 个字符保留 'a'，后面保留 'b'
        for (int i = 0; i <= n; ++i) {
            // 总删除数 = 左侧的 'b'（需要删掉） + 右侧的 'a'（需要删掉）
            min_del = min(min_del, left_b + right_a[i]);
            // 注意：i == n 时 s[i] 越界，需跳过
            if (i < n && s[i] == 'b') {
                left_b++;
            }
        }

        return min_del;
    }
};
