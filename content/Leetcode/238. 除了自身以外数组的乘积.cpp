#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        if (n == 2) return {nums[1], nums[0]};

        vector<int> pre(n, 1); 
        vector<int> suf(n, 1); 
        pre[0] = nums[0]; 
        suf[n - 1] = nums[n - 1]; 

        for (int i = 1; i < n; i++) {
            pre[i] = pre[i - 1] * nums[i];
        }
        for (int i = n - 2; i >= 0; i--) {
            suf[i] = suf[i + 1] * nums[i];
        }

        display(pre);
        display(suf);

        vector<int> ans(n, 1);
        for (int i = 0; i < n; i++) {
            if (i >= 1) ans[i] *= pre[i - 1];
            if (i < n - 1) ans[i] *= suf[i + 1];
        }

        display(ans);
        return ans;
    }

    void display(vector<int> &arr) {
        for (int i = 0; i < arr.size(); i++) 
            printf("%d, ", arr[i]);
        cout << "\n";
    }
};

int main() {
    vector<int> nums = {1,2,3,4}; 
    Solution().productExceptSelf(nums);
}
