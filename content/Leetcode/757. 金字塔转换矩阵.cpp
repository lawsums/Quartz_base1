// 普通回溯搜索
class Solution {
private:
    int n;
    unordered_map<string, vector<char>> mp;
    vector<string> path;

public:
    bool pyramidTransition(string bottom, vector<string>& allowed) {
        n = bottom.size();
        path.assign(n, "");
        path[0] = bottom;
        for (int i = 1; i < n; i++) {
            path[i] = string(n - i, ' '); // 使用空格初始化
        }

        // 构建映射
        for (const auto& elem : allowed) {
            string key = elem.substr(0, 2);
            mp[key].push_back(elem[2]);
        }

        return dfs(1, 0);
    }

    bool dfs(int r, int c) {
        // 这是一种回溯+DFS，用于构建金字塔
        if (r == n) {
            return true; // 成功构建到顶部
        }
        
        // 如果已经处理到当前行的末尾，转到下一行
        if (c == n - r) {
            return dfs(r + 1, 0);
        }
        
        // 获取当前位置需要基于的两个块
        string key = string(1, path[r-1][c]) + string(1, path[r-1][c+1]);
        
        if (!mp.count(key)) {
            return false; // 没有可用的转换规则
        }
        
        // 尝试所有可能的转换
        for (char ch : mp[key]) {
            path[r][c] = ch; // 设置当前块
            
            // 如果当前行还没完成，继续处理下一个位置
            bool result = false;
            if (c + 1 < n - r) {
                result = dfs(r, c + 1);
            } else {
                // 当前行完成，转到下一行
                result = dfs(r + 1, 0);
            }
            
            if (result) return true;
            
            // 回溯 - 不需要显式清除，因为会被后续的赋值覆盖
        }
        
        return false;
    }
};


// 记忆化搜索
class Solution {
private:
    unordered_map<string, vector<char>> rules;
    unordered_map<string, bool> memo; // 记忆化缓存
    
public:
    bool pyramidTransition(string bottom, vector<string>& allowed) {
        // 构建转换规则
        for (const auto& rule : allowed) {
            string key = rule.substr(0, 2);
            rules[key].push_back(rule[2]);
        }
        
        return dfs(bottom);
    }
    
    bool dfs(string current) {
        // 如果当前层只有一个字符，到达顶部
        if (current.size() == 1) return true;
        
        // 检查记忆化缓存
        if (memo.count(current)) return memo[current];
        
        // 生成所有可能的下一层
        vector<string> nextLevels;
        generateAllNextLevels(current, 0, "", nextLevels);
        
        // 对每个可能的下一层进行DFS
        for (const string& next : nextLevels) {
            if (dfs(next)) {
                memo[current] = true; // 缓存结果
                return true;
            }
        }
        
        memo[current] = false; // 缓存结果
        return false;
    }
    
    void generateAllNextLevels(const string& current, int index, 
                              string currentNext, vector<string>& results) {
        // 基准情况：已经处理完所有相邻块
        if (index == current.size() - 1) {
            results.push_back(currentNext);
            return;
        }
        
        // 获取当前相邻的两个字符
        string key = string(1, current[index]) + current[index + 1];
        
        // 如果没有转换规则，直接返回
        if (!rules.count(key)) return;
        
        // 对每个可能的转换字符进行递归
        for (char ch : rules[key]) {
            generateAllNextLevels(current, index + 1, 
                                 currentNext + ch, results);
        }
    }
};
