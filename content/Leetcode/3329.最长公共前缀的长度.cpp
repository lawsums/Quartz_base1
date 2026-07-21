#include <bits/stdc++.h>
#include <string>
using namespace std;


#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

class TrieNode {
public:
    int pass;
    int end;
    vector<TrieNode *> nexts;

    TrieNode() {
        pass = 0;
        end = 0;
        nexts = vector<TrieNode *>(26, nullptr);
    }

    ~TrieNode() {
        for (auto &child: nexts) {
            delete child;
        }
    }
};

class Trie {
private:
    TrieNode *root;

public:
    Trie() {
        root = new TrieNode();
    }

    // 析构函数：释放根节点（会触发整个树的递归释放）
    ~Trie() {
        delete root;   // 根节点的析构函数会释放所有子节点
        root = nullptr;// 避免野指针
    }

    void insert(string word) {
        auto cur = root;
        cur->pass++;
        for (const auto &c: word) {
            auto path = c - 'a';
            // 如果不存在就创建
            if (cur->nexts[path] == nullptr) {
                cur->nexts[path] = new TrieNode();
            }
            cur = cur->nexts[path];
            cur->pass++;
        }
        cur->end++;
    }

    bool search(const string &word) {
        return countWordsEqualTo(word) > 0;
    }

    int searchPrefixLen(const string &word) {
        return countWordsEqualTo(word) > 0;
    }

    bool startsWith(const string &prefix) {
        return countWordsStartingWith(prefix) > 0;
    }

// 这道题里面startsWith和search都是bool类型返回值，不需要暴露以下方法
private:
    // 返回值: [完整单词数量, 前缀数量, 匹配到的长度]
    array<int, 3> count(const string &word) {
        auto cur = root;
        int cnt{};
        for (const auto &c: word) {
            auto path = c - 'a';
            // 如果这个路径不存在直接返回0
            if (cur->nexts[path] == nullptr) {
                return {0, 0, cnt};
            }
            cur = cur->nexts[path];
            cnt++;
        }
        // 最后返回是否是end
        return {cur->end, cur->pass, cnt};
    }

    int countWordsEqualTo(const string &word) {
        return count(word)[0];
        // auto cur = root;
        // for (const auto& c : word) {
        //     auto path = c - 'a';
        //     // 如果这个路径不存在直接返回0
        //     if (cur->nexts[path] == nullptr) {
        //         return 0;
        //     }
        //     cur = cur->nexts[path];
        // }
        // // 最后返回是否是end
        // return cur->end;
    }

    int countWordsStartingWith(const string &word) {
        return count(word)[1];
        // auto cur = root;
        // for (const auto& c : word) {
        //     auto path = c - 'a';
        //     // 如果这个路径不存在直接返回0
        //     if (cur->nexts[path] == nullptr) {
        //         return 0;
        //     }
        //     cur = cur->nexts[path];
        // }
        // // 最后返回pass
        // return cur->pass;
    }

    int countWordPrefixLen(const string &word) {
        return count(word)[2];
    }

    // 如果word存在过才有必要删除，否则不需要删除
    void delete_(const string &word) {
        if (countWordsEqualTo(word) > 0) {
            auto cur = root;
            cur->pass--;

            for (const auto &c: word) {
                auto path = c - 'a';
                // 如果删除完了之后pass == 0, 直接删除后面的整条分支
                if (--cur->nexts[path]->pass == 0) {
                    delete cur->nexts[path];
                    cur->nexts[path] = nullptr;
                    return;
                }
                // pass > 0
                cur = cur->nexts[path];
            }

            cur->end--;
        }
    }
};

class Solution {
public:
    int longestCommonPrefix(vector<int> &arr1, vector<int> &arr2) {
        Trie t;

        for (const auto &num: arr2) {
            t.insert(to_string(num));
        }

        // 用arr1里面的元素去匹配
        int ans = 0;
        for (const auto &num : arr1) {
            ans = max(ans, t.searchPrefixLen(to_string(num)));
        }

        return ans;
    }
};
