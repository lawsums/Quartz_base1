#include <iostream>
#include <vector>
#include <unordered_map>

#define TEST(func, ...) \
    func(__VA_ARGS__);  \
    std::cout << #func << " passed!\n"

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
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ==================== 1. 泛型前缀树节点 ====================
template<size_t SIZE>
class TrieNode {
public:
    int pass{0};
    int end{0};
    int shortest{-1};
    // 使用 unique_ptr 自动管理子节点生命周期，防止任何形式的内存泄漏
    array<unique_ptr<TrieNode>, SIZE> nexts;

    TrieNode() = default;
    // 告别手动递归 delete，默认析构函数会自动释放整个 tree 分支
    ~TrieNode() = default;
};

// ==================== 2. 泛型前缀树核心类 ====================
template<size_t SIZE, typename Transformer>
class GenericTrie {
private:
    using Node = TrieNode<SIZE>;
    unique_ptr<Node> root;
    Transformer transform;// 字符转索引的策略对象

public:
    GenericTrie() : root(make_unique<Node>()) {}
    ~GenericTrie() = default;

    // 插入单词
    void insert(const string &word, int ind, vector<string> &wc) {
        auto *cur = root.get();
        cur->pass++;
        // 更新shortest的位置
        if (cur->shortest == -1) {
            cur->shortest = ind;
        } else {
            if (wc[ind].size() < wc[cur->shortest].size() || (wc[ind].size() == wc[cur->shortest].size() &&ind < cur->shortest)) {
                cur->shortest = ind;
            } 
        }

        for (const auto &c: word) {
            size_t path = transform(c);
            // 安全边界检查，防止坏输入压垮系统的 OOB
            if (path >= SIZE) continue;

            if (cur->nexts[path] == nullptr) {
                cur->nexts[path] = make_unique<Node>();
            }
            cur = cur->nexts[path].get();
            cur->pass++;
            // 更新shortest的位置
            if (cur->shortest == -1) {
                cur->shortest = ind;
            } else {
                if (wc[ind].size() < wc[cur->shortest].size() || (wc[ind].size() == wc[cur->shortest].size() &&ind < cur->shortest)) {
                    cur->shortest = ind;
                } 
            }

            LV(c, ind, cur->shortest);
        }
        cur->end++;
    }

    bool search(const string &word) {
        return countWordsEqualTo(word) > 0;
    }

    int searchPrefixLen(const string &word) {
        return countWordPrefixLen(word);
    }

    bool startsWith(const string &prefix) {
        return countWordsStartingWith(prefix) > 0;
    }

    // 支持删除操作（顺便用模板写全）
    void remove(const string &word) {
        if (countWordsEqualTo(word) == 0) return;

        auto *cur = root.get();
        cur->pass--;
        for (const auto &c: word) {
            size_t path = transform(c);
            if (path >= SIZE) return;

            // 如果某条路径减一后 pass 归零，直接用 reset() 斩断并释放整条分支
            if (--cur->nexts[path]->pass == 0) {
                cur->nexts[path].reset();
                return;
            }
            cur = cur->nexts[path].get();
        }
        cur->end--;
    }

    int countLongestPrefixInd(const string &word) {
        auto *cur = root.get();

        for (const auto &c: word) {
            LV("countPrefix", c);

            size_t path = transform(c);
            if (path >= SIZE || cur->nexts[path] == nullptr) {
                return cur->shortest;
            }
            cur = cur->nexts[path].get();
        }
        return cur->shortest;
    }

private:
    // 统一查询核心：返回值 {完整单词数, 前缀数, 匹配的最大字符长度}
    array<int, 3> count(const string &word) {
        auto *cur = root.get();
        int cnt = 0;
        for (const auto &c: word) {
            size_t path = transform(c);
            if (path >= SIZE || cur->nexts[path] == nullptr) {
                return {0, 0, cnt};
            }
            cur = cur->nexts[path].get();
            cnt++;
        }
        return {cur->end, cur->pass, cnt};
    }

    int countWordsEqualTo(const string &word) { return count(word)[0]; }
    int countWordsStartingWith(const string &word) { return count(word)[1]; }
    int countWordPrefixLen(const string &word) { return count(word)[2]; }
};

// ==================== 3. 策略转换器定义（三大金刚） ====================

// ① 小写字母转换器 (a-z)
struct AlphabetTransformer {
    size_t operator()(char c) const { return static_cast<size_t>(c - 'a'); }
};

// ② 数字转换器 (0-9)
struct DigitTransformer {
    size_t operator()(char c) const { return static_cast<size_t>(c - '0'); }
};

// ③ 完整 ASCII 转换器 (0-127)
struct AsciiTransformer {
    size_t operator()(char c) const { return static_cast<size_t>(static_cast<unsigned char>(c)); }
};

// ==================== 4. 类型别名（对外提供干净的类名） ====================
using AlphabetTrie = GenericTrie<26, AlphabetTransformer>;
using DigitTrie = GenericTrie<10, DigitTransformer>;
using AsciiTrie = GenericTrie<128, AsciiTransformer>;

class Solution {
public:
    vector<int> stringIndices(vector<string> &wordsContainer, vector<string> &wordsQuery) {
        // 反向加入trie
        AlphabetTrie t;
        for (int i = 0; i < wordsContainer.size(); ++i) {
            string reversed_word(wordsContainer[i].rbegin(), wordsContainer[i].rend());
            t.insert(reversed_word, i, wordsContainer);
        }

        // 问题在于怎么快速判断 最长公共后缀 且 最短 的答案
        vector<int> ans;
        for (const auto &word : wordsQuery) {
            string reversed_word(word.rbegin(), word.rend());
            ans.push_back(t.countLongestPrefixInd(reversed_word));
        }

        return ans;
    }
};

