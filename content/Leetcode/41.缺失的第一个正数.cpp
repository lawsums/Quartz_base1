class Solution {
public:
    int firstMissingPositive(vector<int>& nums) {
        int n = nums.size();
        // 将所有小于等于0的数变成n + 1;
        for (int i = 0; i < n; i++) {
            if (nums[i] <= 0) nums[i] = n + 1;
        }
        
        // 将所有<= n的数对应的下标[num - 1]变成负数
        for (int i = 0; i < n; i++) { 
            // 这里要abs防止之前的负号影响运算
            int num = abs(nums[i]);
            // 因为此时所有数>= 1, 所以不会非法访问
            if (num <= n) nums[num - 1] = -abs(nums[num - 1]);
        }

        // 找到第一个正数, 返回下标加1
        for (int i = 0; i < n; i++) {
            if (nums[i] > 0) return i + 1;
        }

        return n + 1;
    }
};
