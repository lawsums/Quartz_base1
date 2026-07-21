#include <bits/stdc++.h>
using namespace std;


class Trie {
public:
    void insert(string word) {
        auto cur = root; 
        cur->pass++;
        for (const auto &c : word) {
            auto path = c - 'a';
            if (cur->children[path] == nullptr) { // 如果没有就创建这个路径
                cur->children[path] = new TrieNode();
            }
            cur = cur->children[path]; // 移动到下一层
            cur->pass++; // 途径++
        }
        cur->end++; // 结尾++
    }

    bool isNotPrefix(string word) {
        auto cur = root; 
        for (const auto &c : word) {
            auto path = c - 'a';
            if (cur->children[path] == nullptr) { // 如果没有这个路径直接返回false
                return false;
            }
            cur = cur->children[path]; // 移动到下一层
        }
        // 结尾到达结尾之后返回答案
        return cur->pass == cur->end;
    }

    class TrieNode {
    public:
        int pass;
        int end;
        vector<TrieNode*> children;

        TrieNode() {
            pass = 0; // 重点!!!!!
            end = 0;
            children.assign(26, nullptr); // 首先赋值成nullptr节省内存
        }
    };

    TrieNode *root;

    Trie() { // TODO 没有释放内存
        root = new TrieNode();
    }

};

class Solution {
public:
    int minimumLengthEncoding(vector<string>& words) {
        // 排序去重
        sort(words.begin(), words.end());
        auto it = unique(words.begin(), words.end());
        words.erase(it, words.end());

        Trie t;
        for (auto& word: words) {
            reverse(word.begin(), word.end()); // 反转word
            t.insert(word); // 插入word
        }

        // 反向检测所有word是否是另一个单词的前缀
        // 如果是另一个单词的前缀的话, 就不需要加入答案
        // 否则ans += word.size() + 1;
        int ans = 0;
        for (auto &word: words) {
            if (t.isNotPrefix(word)) {
                ans += word.size() + 1;
            } 
        }
        return ans;
    }
};

