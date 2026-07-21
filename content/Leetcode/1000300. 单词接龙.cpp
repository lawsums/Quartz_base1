#include <bits/stdc++.h>
using namespace std;
using aii = array<int, 2>;

class Solution {
private:
    unordered_map<string, int> str_to_index; 

public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        // 检查 endWord 是否在 wordList 中
        if (find(wordList.begin(), wordList.end(), endWord) == wordList.end()) {
            return 0;
        }
        
        // 将 beginWord 添加到 wordList 中以便建图
        wordList.push_back(beginWord);
        int n = wordList.size();
        
        vector<int> distance(n, INT_MAX);
        // vector<bool> visited(n, false);
        bool visited[n];
        memset(visited, 0, n * sizeof(bool)); // 分配位置，值，长度
        vector<vector<aii>> graph(n, vector<aii>{});
        
        build(graph, wordList);
        
        // 找到 beginWord 的索引（现在是 wordList 的最后一个元素）
        int startIdx = n - 1;
        distance[startIdx] = 1; // 注意：起点长度为1，不是0
        
        // Dijkstra 算法
        auto cmp = [](aii& a, aii& b) { return a[1] > b[1]; };
        priority_queue<aii, vector<aii>, decltype(cmp)> pq(cmp);
        pq.push({startIdx, 1});
        
        while (!pq.empty()) {
            int u = pq.top()[0]; 
            pq.pop();
            
            if (visited[u]) continue;
            visited[u] = true;
            
            // 如果找到目标单词，直接返回
            if (wordList[u] == endWord) {
                return distance[u];
            }
            
            for (const auto &edge : graph[u]) {
                auto [v, w] = edge;
                int newDist = distance[u] + 1; // 每条边权重是1，但要计算节点数
                
                if (!visited[v] && newDist < distance[v]) {
                    distance[v] = newDist;
                    pq.push({v, distance[v]});
                }
            }
        }
        
        // 找到 endWord 的索引
        int endIdx = -1;
        for (int i = 0; i < n - 1; i++) { // 注意：n-1 因为最后一个是 beginWord
            if (wordList[i] == endWord) {
                endIdx = i;
                break;
            }
        }
        
        return distance[endIdx] == INT_MAX ? 0 : distance[endIdx];
    }

    void build(vector<vector<aii>> &graph, vector<string> &wordList) {
        int n = wordList.size();
        
        // 构建字符串到索引的映射
        for (int i = 0; i < n; i++) {
            str_to_index[wordList[i]] = i;
        }

        // 建图
        for (int i = 0; i < n; i++) {
            const string& word = wordList[i];
            for (int j = 0; j < word.size(); j++) {
                string new_word = word;
                for (char c = 'a'; c <= 'z'; c++) {
                    if (c == word[j]) continue;
                    new_word[j] = c;
                    
                    // 如果新单词在字典中，并且不是自己
                    if (str_to_index.count(new_word) && new_word != word) {
                        graph[i].push_back({str_to_index[new_word], 1});
                    }
                }
            }
        }
    }
};
