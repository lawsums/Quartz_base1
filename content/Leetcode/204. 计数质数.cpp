#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    vector<bool> isPrime; 

public:
    int countPrimes(int n) {
        // 多预留一个位置， 方便下标操作
        isPrime.assign(n, true);
        // 使用埃氏筛
        for (int i = 2; i * i < n /* 只有可能在这个范围内有因子 */; i++) {
            if (isPrime[i]) {
                for (int k = i * i /* 自己不是质数， 而(i - 1) * i 这样的数之前的质数已经标记过了 */; k < n; k += i) {
                    // 标记为合数
                    isPrime[k] = false;
                }
            }
        } 

        return check(isPrime);
    }

    int check(vector<bool> &isPrime) {
        int ans = 0;
        for (int i = 2; i < isPrime.size(); i++) {
            ans += isPrime[i] ? 1 : 0;
        } 
        return ans;
    } 
};
