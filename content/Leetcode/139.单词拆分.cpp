#include <bits/stdc++.h>
using namespace std;

class Trie {
public:
    struct TrieNode {
        int pass;
        int end;
        TrieNode *children[26];

        TrieNode() : pass(0), end(0) {
            for (int i = 0; i < 26; i++)  // 初始化为nullptr
                children[i] = nullptr;
        }
    };

    Trie() {
        root = new TrieNode(); 
    }

    /**********************************/
    // 修复析构函数：递归删除所有节点
    ~Trie() {
        destroy(root);
    }

    // 递归销毁所有Trie节点
    void destroy(TrieNode* node) {
        if (node == nullptr) return;
        // 先递归删除所有子节点
        for (int i = 0; i < 26; i++) {
            destroy(node->children[i]);
        }
        // 再删除当前节点
        delete node;
    }
    /**********************************/

    void insert(string word) {
        auto cur = root;
        cur->pass++;
        for (const auto &c : word) {
            auto path = c - 'a';
            if (cur->children[path] == nullptr) { // 创建这个路径
                cur->children[path] = new TrieNode();
            }
            // 添加途径次数
            cur = cur->children[path];
            cur->pass++;
        }
        // 表示这个单词结尾了
        cur->end++;
    }

    bool search(string word) const {
        auto cur = root;
        for (const auto &c : word) {
            auto path = c - 'a';
            // 如果路径不存在直接返回false
            if (cur->children[path] == nullptr) return false; 
            cur = cur->children[path];
        }
        // 检测是否是以这个字母结尾
        return cur->end > 0;
    }

    bool search(string &word, int l, int r) const {
        // ***防止越界***
        if (r >= word.size()) return false;
        auto cur = root;
        for (int i = l; i <= r; i++) {
            auto c = word[i]; 
            auto path = c - 'a';
            // 如果路径不存在直接返回false
            if (cur->children[path] == nullptr) return false; 
            cur = cur->children[path];
        }
        // 检测是否是以这个字母结尾
        return cur->end > 0;
    }

private:
    TrieNode *root;

};

class Solution {
private:
    // 创建一个栈上的对象, 自动删除
    // TODO 栈上对象会调用构造函数吗 
    Trie t;
    unordered_map<int, bool> memo;


public:
    bool wordBreak(string s, vector<string>& wordDict) {
        
        // 插入单词
        for (const auto &word :wordDict) {
            t.insert(word);
        }
        
        // 通过dfs函数去获取答案
        bool ans = dfs(0, s, wordDict);
        // print(memo);
        return ans;
    }

    bool dfs(int dep, string &s, vector<string> &words) {
        // print(dep);

        // 如果匹配得超出长度了直接返回false
        if (dep > s.size()) return false;
        // 如果正好就返回true
        if (dep == s.size()) return true;

        // 记忆化搜索
        if (memo.count(dep)) return memo[dep];

        // 不是找有没有单词啊, 是找当前相关的单词
        for (const auto &word : words) {
            // if (!t.search(s, dep, dep + word.size() - 1)) continue; // 如果没找到直接下一个单词
            if (dep + word.size() > s.size() || !t.search(s.substr(dep, word.size()))) continue; // 如果没找到直接下一个单词

            // 如果成功找到当前单词可以去后面继续搜索, 同时这里启用一个记忆化搜索
            if (dfs(dep + word.size(), s, words)) {
                memo[dep] = true;
                return true;
            }
        }

        // 如果都没找到返回false
        memo[dep] = false;
        return false;
    }
};


