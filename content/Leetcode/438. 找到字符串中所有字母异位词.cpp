#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        return f1(s, p);
    }


    // 哈希表遍历
    vector<int> f1(string s, string p) {
        unordered_map<int, vector<int>> mp; 

        int n = s.size(), m = p.size();
        vector<int> path(26, 0);
        // 初始化
        mp[-1] = path;

        // 构建p的词频
        vector<int> p_cnt(26, 0);
        for (auto c : p) {
            p_cnt[c - 'a']++;
        }

        // 构造前缀和, 更新答案
        vector<int> ans;
        for (int i = 0; i < n; i++) {
            path[s[i] - 'a']++;
            mp[i] = path;
            if (i >= m - 1) {
                if (check(mp, i, p, p_cnt)) ans.push_back(i - m + 1);
            }
        }

        return ans;
    }

private:
    bool check(unordered_map<int, vector<int>> &mp, int id, string &p, vector<int> &p_cnt) {
        int m = p.size();
        vector<int> cur = mp[id], pre = mp[id - m];
        // 检测词频是否相同
        for (int i = 0; i < 26; i++) {
            if (cur[i] - pre[i] != p_cnt[i]) return false; 
        }
        return true;
    }

public:
    // 欠债模型
    vector<int> f2(string s, string p) {
        int n = s.size(), m = p.size();
        vector<int> ans;
        if (n < m) return ans;

        vector<int> cnt(26, 0); 
        // 首先欠债
        for (int i = 0; i < m; i++) {
            // TODO 这里的欠债为什么不要考虑cnt里面是否大于0, 我记得有的题目里面好像要考虑
            // ANS: 
            // 一、先明确：这道题的「欠债模型」核心逻辑
            // 这道题是找异位词，本质要求是：窗口内的字符种类、数量，和 p 完全一致（不多、不少、不差）。我们定义的cnt[26]是 **「字符的供需差值」**，核心含义：
            // cnt[i] > 0：字符i+'a'缺这么多个（p 需要，但窗口里不够）
            // cnt[i] < 0：字符i+'a'多这么多个（窗口里有，但 p 不需要 / 多出来了）
            // cnt[i] = 0：字符i+'a'供需平衡（窗口里的数量和 p 完全一致）
            // 所以当 cnt 全为 0 时，窗口就是 p 的异位词—— 这是这道题的核心结论，也是为什么不需要单独判断 cnt>0的原因：
            // 全 0 的条件已经同时包含了「不缺（无 cnt>0）」和「不多（无 cnt<0）」，单独判断反而多余。
            // 你代码里f2的逻辑是完全正确的，测试用例s="cbaebabacd", p="abc"会正确输出[0,6]。
            // 二、关键：什么时候需要判断cnt>0？
            // 需要判断cnt>0的场景，是另一类和异位词相似但要求不同的题目：找字符串中包含 p 所有字符的子串 / 子序列（即「最小覆盖子串」问题）。比如力扣 76 题「最小覆盖子串」：要求找到 s 中包含 p 所有字符（数量也匹配）的最短子串，此时窗口的要求是：
            // 窗口内的字符，对于 p 需要的字符，数量必须≥p 的数量（可以多，不能少）
            // 其他字符可以任意多
            // 此时我们的欠债模型会做优化：只统计「缺的字符数」，此时需要判断cnt[i] > 0（表示还缺这个字符）
            cnt[p[i] - 'a']++;
            cnt[s[i] - 'a']--;
        }

        auto all_zero = [](const vector<int> &arr) { 
            return all_of(arr.begin(), arr.end(), [](int a) { return a == 0; });
        };
                
        if (all_zero(cnt)) ans.push_back(0);

        for (int i = m; i < n; i++) {
            cnt[s[i] - 'a']--;
            cnt[s[i - m] - 'a']++;
            if (all_zero(cnt)) ans.push_back(i - m + 1);
        }

        return ans;
    }
};

