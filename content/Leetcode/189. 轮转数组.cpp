#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    // copy_forward整体移动 O(n), O(n)
    void rotate(vector<int> &nums, int k) {
        f4(nums, k);
    }

    void f1(vector<int> &nums, int k) {
        int n = nums.size();
        if (n == 0 || k == 0) return;// 边界条件：空数组或k=0时直接返回
        k = k % n;                   // 处理k大于数组长度的情况
        int offset = n - k;

        // 保存最后k个需要旋转到前面的元素
        vector<int> need(nums.begin() + offset, nums.end());

        // 修复：使用正确的迭代器调用copy_backward，将前n-k个元素向后移动k位
        // 源区间：[nums.begin(), nums.end()-k)，目标结束位置：nums.end()
        std::copy_backward(nums.begin(), nums.end() - k, nums.end());

        // 将保存的最后k个元素放到数组前k个位置
        for (int i = 0; i < k; i++) {
            nums[i] = need[i];
        }
    }

    // insert O(n^2), O(1)
    void f2(vector<int> &nums, int k) {
        int n = nums.size();
        if (n == 0 || k == 0) return;// 边界条件：空数组或k=0时直接返回
        k = k % n;                   // 处理k大于数组长度的情况

        while (k--) {
            nums.insert(nums.begin(), nums.back());
            nums.pop_back();
        }
    }

    // list O(n), O(n)
    void f3(vector<int> &nums, int k) {
        // 将 vector 转换为 list（模拟链表操作）
        std::list<int> lst(nums.begin(), nums.end());
        int n = lst.size();
        if (n == 0 || k == 0) return;

        // 关键：k 取模，减少无效轮转
        k = k % n;

        // 向右轮转 k 步 = 把最后 k 个元素依次移到头部
        for (int i = 0; i < k; ++i) {
            // 取出最后一个元素
            int last = lst.back();
            // 删除最后一个元素
            lst.pop_back();
            // 将该元素插入到头部
            lst.push_front(last);
        }

        // 将 list 转回原 vector（完成结果覆盖）
        nums.assign(lst.begin(), lst.end());
    }

    // 反转三次 O(n), O(1)
    void f4(vector<int> &nums, int k) {
        int n = nums.size();
        if (n == 0 || k == 0) return;
        k = k % n;

        // 步骤1：反转整个数组
        std::reverse(nums.begin(), nums.end());
        // 步骤2：反转前 k 个元素
        std::reverse(nums.begin(), nums.begin() + k);
        // 步骤3：反转剩余 n-k 个元素
        std::reverse(nums.begin() + k, nums.end());
    }
};
