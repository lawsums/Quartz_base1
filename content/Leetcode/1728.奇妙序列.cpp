#define MOD 1'000'000'007

class Fancy {
private:
    vector<long long> nums;  // 存储反向计算后的数值
    long long add = 0;       // 全局加法偏移量
    long long mul = 1;       // 全局乘法因子

    // 快速幂计算 (a^b) % MOD
    long long pow_mod(long long a, long long b) {
        long long res = 1;
        a %= MOD;
        while (b > 0) {
            if (b & 1) {
                res = (res * a) % MOD;
            }
            a = (a * a) % MOD;
            b >>= 1;
        }
        return res;
    }

    // 计算乘法逆元 (mod为质数，用费马小定理)
    long long inv(long long x) {
        return pow_mod(x, MOD - 2);
    }

public:
    Fancy() {}
    
    void append(int val) {
        // 反向抵消当前的add和mul，得到存储值
        // 公式推导：存储值 * mul + add = val → 存储值 = (val - add) * inv(mul)
        long long num = (val - add + MOD) % MOD;  // 加MOD避免负数
        num = num * inv(mul) % MOD;
        nums.push_back(num);
    }
    
    void addAll(int inc) {
        // 全局加法：所有元素 += inc → 等价于 add += inc
        add = (add + inc) % MOD;
    }
    
    void multAll(int m) {
        // 全局乘法：所有元素 *= m → 等价于 mul *= m, add *= m
        mul = (mul * m) % MOD;
        add = (add * m) % MOD;
    }
    
    int getIndex(int idx) {
        // 下标越界返回-1
        if (idx < 0 || idx >= nums.size()) {
            return -1;
        }
        // 计算真实值：存储值 * mul + add
        long long res = (nums[idx] * mul + add) % MOD;
        return (int)res;
    }
};

/**
 * Your Fancy object will be instantiated and called as such:
 * Fancy* obj = new Fancy();
 * obj->append(val);
 * obj->addAll(inc);
 * obj->multAll(m);
 * int param_4 = obj->getIndex(idx);
 */
