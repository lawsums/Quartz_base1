#include <bits/stdc++.h>
using namespace std;


class TrieNode {
public:
    int pass;
    int end;
    vector<TrieNode*> nexts;

    TrieNode() {
        pass = 0;
        end = 0;
        nexts = vector<TrieNode*>(26, nullptr);
    }

    ~TrieNode() {
        for (auto& child : nexts) {
            delete child;
        }
    }
};

class MagicDictionary {
private:
    TrieNode *root;

public:
    /** Initialize your data structure here. */
    MagicDictionary() {
        root = new TrieNode();
    }
    
    void insert(string word) {
        auto cur = root;
        cur->pass++;
        for (const auto& c : word) {
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

    void buildDict(vector<string> dictionary) {
        for (const auto &s : dictionary) {
            insert(s); // 插入所有的字符串
        }
    }
    
    bool search(string searchWord) {
        return handler(searchWord, root, true, 0);
    }

    bool handler(string &searchWord, TrieNode* cur, bool canReplace, int step) {
        if (step == searchWord.size()) return cur->end && !canReplace;

        auto path = searchWord[step] - 'a'; // 设置当前路径
        bool found = false;

        if (cur->nexts[path] != nullptr) {  // 如果当前路径存在
            found |= handler(searchWord, cur->nexts[path], canReplace, step + 1);
        } 

        if (!found && canReplace) { // 如果还没找到而且当前可以替换, 则尝试替换成其他25个字母
            for (int i = 0; i < 26; i++) {
                if (i == path || cur->nexts[i] == nullptr) continue;
                found |= handler(searchWord, cur->nexts[i], false, step + 1);
            }
        } 

        return found;
    }
};