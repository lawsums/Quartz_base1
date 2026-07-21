#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

class Solution {
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
 // 输入：n = 2, logs = ["0:start:0","0:start:2","0:end:5","1:start:6","1:end:6","0:end:7"]
// 输出：[7,1]
        vector<int> ans(n, 0);
        vector<pair<int, int>> stk; // [time, id]
        for (const auto& log : logs) {
            vector<string> result = split(log, ':');
            // 对于每一个log, 我们获取他的[ 序号, start/end, 开始/结束时间 ]
            int id = stoi(result[0]);
            int time = stoi(result[2]);
            if (result[1] == "start") {
                // 如果之前还有别的函数需要暂时结束独占
                if (!stk.empty()) ans[stk.back().second] += time /* 因为都是开始时间不需要+1 */ - stk.back().first; 
                stk.push_back({time, id}); // 推入当前时间
            } else { // 说明结束了
                // pop一个获取之前的开始时间
                ans[id] += (time + 1) - stk.back().first; 
                stk.pop_back();
                // 下一个时刻给上一个栈中的函数
                if (!stk.empty()) stk.back().first = time + 1;
            }
        }

        return ans;
    }

    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};




