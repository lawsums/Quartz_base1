#include <iostream>
#include <vector>
#include <unordered_map>

// 1. 设置模式开关：提交时将下面的 1 改为 0 即可
#define DEBUG_MODE 0

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

#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

class UnionFind {
public:
    vector<int> parent;  // 父节点数组
    vector<int> rank;    // 秩（树的高度）数组

public:
    // 初始化：size 为元素个数
    UnionFind(int size = 1'000'001) {
        parent.resize(size);
        rank.resize(size, 0);
        // 每个元素初始父节点是自己
        for (int i = 0; i < size; ++i) {
            parent[i] = i;
        }
    }

    // 查找根节点 + 路径压缩
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); // 路径压缩
        }
        return parent[x];
    }

    // 合并两个节点，返回是否成功合并
    // false：已经在同一个集合
    // true：合并成功
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        // 同一个集合，无需合并
        if (rootX == rootY) {
            return false;
        }

        // 按秩合并：小树挂到大树上
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else {
            parent[rootY] = rootX;
            // 秩相同，合并后高度 +1
            if (rank[rootX] == rank[rootY]) {
                rank[rootX]++;
            }
        }
        return true;
    }
};

class Solution {
public:
    bool hasValidPath(vector<vector<int>>& grid) {
        int n = grid.size(), m = grid[0].size();
        UnionFind uf(n * m);

        auto pos = [=](int i, int j) {
            return i * m + j;
        };

        auto tackle = [&](int i, int j) {
            int r = grid[i][j];
            if (r == 1 || r == 4 || r == 6) {
                if (j + 1 < m) {
                    int rr = grid[i][j + 1];
                    if (rr == 1 || rr == 3 || rr == 5) {
                        uf.unite(pos(i, j), pos(i, j + 1));
                    }
                }
            } 
            // 应该两个都要考虑
            if (r == 2 || r == 3 || r == 4) {
                if (i + 1 < n) {
                    int rd = grid[i + 1][j];
                    if (rd == 2 || rd == 5 || rd == 6) {
                        uf.unite(pos(i, j), pos(i + 1, j));
                    }
                }
            }
        };
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                tackle(i, j);
                LV(uf.parent);
            }
        }

        return uf.find(0) == uf.find(pos(n - 1, m - 1));
    }
};

