#include <bits/stdc++.h>
using namespace std;

#include <iostream>
#include <vector>
#include <unordered_map>

#define TEST(func, ...) \
    func(__VA_ARGS__);  \
    std::cout << #func << " passed!\n"

// 1. 设置模式开关：提交时将下面的 1 改为 0 即可
#define DEBUG_MODE 1

#if DEBUG_MODE
    #define LOG(x) std::cout << "[DEBUG] " << x << std::endl
    
    // 条件打印：只有 cond 为真时才输出内容 x
    #define CLOG(cond, x) do { if(cond) std::cout << "[COND] " << x << std::endl; } while(0)

    // 多参数 LV 核心实现：支持 1-5 个参数
    #define GET_LV(_1, _2, _3, _4, _5, NAME, ...) NAME
    #define LV(...) std::cout << "[DEBUG] "; GET_LV(__VA_ARGS__, LV5, LV4, LV3, LV2, LV1)(__VA_ARGS__)

    #define LV1(x) std::cout << #x << "=" << (x) << std::endl
    #define LV2(x, ...) std::cout << #x << "=" << (x) << " | "; LV1(__VA_ARGS__)
    #define LV3(x, ...) std::cout << #x << "=" << (x) << " | "; LV2(__VA_ARGS__)
    #define LV4(x, ...) std::cout << #x << "=" << (x) << " | "; LV3(__VA_ARGS__)
    #define LV5(x, ...) std::cout << #x << "=" << (x) << " | "; LV4(__VA_ARGS__)
#else
    #define LOG(x)
    #define CLOG(cond, x)
    #define LV(...)
#endif

// 2. 提供对常用容器的打印支持 (保持原样)
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) os << v[i] << (i == v.size() - 1 ? "" : ", ");
    return os << "]";
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& [k, v] : m) {
        if (!first) os << ", ";
        os << k << ":" << v;
        first = false;
    }
    return os << "}";
}

using ll = long long;

class Solution {
public:
    char processStr(string s, long long k) {
        vector<ll> len{}; 
        len.assign(s.size(), 0);

        for (int i = 0; i < s.size(); ++i) {
            auto c = s[i];

            if (isalpha(c)) {
                len[i] = (i > 0 ? len[i - 1] : 0) + 1;
            } else if (c == '*') {
                if (i == 0 || len[i - 1] == 0) {
                    len[i] = 0; 
                    continue;
                }
                len[i] = len[i - 1]  - 1;
            } else if (c == '#') {
                len[i] = (i > 0 ? len[i - 1] : 0) * 2;
            } else if (c == '%') {
                len[i] = (i > 0 ? len[i - 1] : 0);
            }
        }

        // LV(len);

        for (int i = s.size() - 1; i >= 0; --i) {
            auto c = s[i];

            if (k >= len[i]) return '.';

            if (isalpha(c)) {
                // 弹出一个字符，如果此时长度-1正好是k的话说明可以直接返回了
                if (len[i] - 1 == k) {
                    return c;
                }
            } else if (c == '*') {
                continue;
            } else if (c == '#') {
                // 说明当前字符串要缩水一半
                k = k % (len[i] / 2); 
            } else if (c == '%') {
                // 说明当前k需要绕中心翻转
                k = (len[i] - 1) - k;
            }
        }

        return '.';
    }
};


