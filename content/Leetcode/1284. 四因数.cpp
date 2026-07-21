#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int sumFourDivisors(vector<int>& nums) {
        int ans = 0;
        for (const auto &num : nums) {
            ans += f1(num);
        }

        return ans;
    }

    int f1(int n) {
        if (n < 4) return 0;
        int sum = 0;
        int count = 0;
        for (int i = 1; i * i <= n; ++i) {
            if (n % i == 0) {
                // 如果是完全平方数， 因数个数为奇数不可能有4个因数
                if (i == n / i) return 0;
                else {
                    sum += i + n / i;
                    count += 2;
                    if (count > 4) {
                        // 因数个数超过4, 直接返回0
                        return 0;
                    }
                }
            }
        }

        return count == 4 ? sum : 0;
    }
};
