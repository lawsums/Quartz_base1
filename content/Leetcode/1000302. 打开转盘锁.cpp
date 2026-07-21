#include <bits/stdc++.h>
#include <queue>
using namespace std;

class Solution {
public:
    int openLock(vector<string>& deadends, string target) {
        return f1(deadends, target);
    }

    // 正确做法
    int f1(vector<string>& deadends, string target) {
        // 快速查找死亡数字：用unordered_set比map更高效（只需判断存在性）
        unordered_set<string> deads(deadends.begin(), deadends.end());
        string start = "0000";
        
        // 边界条件：起点或目标是死亡数字，直接返回-1
        if (deads.count(start)) return -1;
        if (deads.count(target)) return -1;
        // 起点就是目标，步数为0
        if (start == target) return 0;
        
        // 双向BFS：两个队列+两个访问集合
        queue<string> q1;  // 起点方向队列
        queue<string> q2;  // 终点方向队列
        unordered_set<string> vis1;  // 起点方向已访问
        unordered_set<string> vis2;  // 终点方向已访问
        
        q1.push(start);
        vis1.insert(start);
        q2.push(target);
        vis2.insert(target);
        
        int step = 0;  // 总步数
        
        // 处理一层节点的函数（传递队列和对应的访问集合，返回是否相遇）
        auto bfsLayer = [&](queue<string>& q, unordered_set<string>& visSelf, unordered_set<string>& visOther) -> bool {
            int size = q.size();
            for (int i = 0; i < size; ++i) {
                string curr = q.front();
                q.pop();
                
                // 生成当前节点的8个邻接节点（4个拨轮，每个拨轮2种方向）
                for (int j = 0; j < 4; ++j) {
                    // 拨轮+1（0->1, ..., 9->0）
                    string next1 = curr;
                    next1[j] = (next1[j] == '9') ? '0' : next1[j] + 1;
                    // 拨轮-1（0->9, ..., 9->8）
                    string next2 = curr;
                    next2[j] = (next2[j] == '0') ? '9' : next2[j] - 1;
                    
                    // 检查next1是否相遇（存在于另一个方向的访问集合）
                    if (visOther.count(next1)) return true;
                    // 若next1不是死亡数字且未被当前方向访问过，入队并标记
                    if (!deads.count(next1) && !visSelf.count(next1)) {
                        visSelf.insert(next1);
                        q.push(next1);
                    }
                    
                    // 检查next2是否相遇
                    if (visOther.count(next2)) return true;
                    // 若next2不是死亡数字且未被当前方向访问过，入队并标记
                    if (!deads.count(next2) && !visSelf.count(next2)) {
                        visSelf.insert(next2);
                        q.push(next2);
                    }
                }
            }
            return false;
        };
        
        // 双向BFS主循环：每次处理一个方向的一层，步数+1
        while (!q1.empty() && !q2.empty()) {
            step++;
            // 优先处理节点少的队列（优化：减少每层处理的节点数）
            if (q1.size() <= q2.size()) {
                // 处理起点方向的一层，检查是否与终点方向相遇
                if (bfsLayer(q1, vis1, vis2)) return step;
            } else {
                // 处理终点方向的一层，检查是否与起点方向相遇
                if (bfsLayer(q2, vis2, vis1)) return step;
            }
        }
        
        // 队列为空仍未相遇，无法解锁
        return -1;
    }

    // 我的做法
    int f2(vector<string>& deadends, string target) {
        int ans = 0;
        unordered_map<string, int> visited;
        unordered_map<string, int> deads;
        for (const auto &de : deadends) { // deadends添加到字典中
            deads[de]++;
        }

        queue<string> q1, q2;
        q1.push("0000");
        q2.push(target);

        auto tackle = [&](queue<string> &q){
            int n1 = q.size();
            for (int i = 0; i < n1; i++) {
                string u = q.front(); q.pop();
                // 如果是deadends就continue
                if (deads.count(u)) continue;
                // 检测当前字符串是否被访问过, 如果访问过, 说明双向bfs成功
                if (visited.count(u)) return true; // 找到了就返回ans 
                visited[u]++;
                for (int j = 0; j < 4; j++) { // 
                    string v1 = u;
                    string v2 = u;
                    v1[j] == '9' ? v1[j] = '0' : v1[j]++; // 如果是9的话只能进位转到0
                    v2[j] == '0' ? v2[j] = '9' : v2[j]--; // 如果是0只能退位转到9
                    q.push(v1);
                    q.push(v2);
                }
            }
            ans++; // 只有一层都遍历完之后再ans++
            return false;
        };

        // 双向bfs
        while (!q1.empty() && !q2.empty()) {
            if (!q1.empty()) {
                if (tackle(q1)) return ans;
            } else { // 如果队列为空了说明deadends把路堵死了
                break; 
            }
            if (!q2.empty()) {
                if (tackle(q2)) return ans;
            } else {
                break;
            }
        }

        return -1; // 没找到

    }
 
	// 更简洁的写法
    int f3(vector<string>& deadends, string target) {
        unordered_set<string> dead(deadends.begin(), deadends.end());
        string start = "0000";
        
        // 边界条件
        if (dead.count(start) || dead.count(target)) return -1;
        if (start == target) return 0;
        
        // 双向BFS初始化
        unordered_set<string> q1 = {start}, q2 = {target}, visited;
        int step = 0;
        
        // BFS函数：处理一层
        auto expand = [&](unordered_set<string>& q, unordered_set<string>& other) -> bool {
            unordered_set<string> next_q;
            for (string cur : q) {
                // 生成所有可能的下一状态
                for (int i = 0; i < 4; i++) {
                    for (int d = -1; d <= 1; d += 2) {
                        string next = cur;
                        next[i] = (next[i] - '0' + d + 10) % 10 + '0';
                        
                        // 检查是否相遇
                        if (other.count(next)) return true;
                        
                        // 检查是否合法
                        if (!dead.count(next) && !visited.count(next)) {
                            next_q.insert(next);
                            visited.insert(next);
                        }
                    }
                }
            }
            q = next_q;
            return false;
        };
        
        // 双向BFS主循环
        while (!q1.empty() && !q2.empty()) {
            step++;
            
            // 优先扩展较小的队列（优化）
            if (q1.size() > q2.size()) swap(q1, q2);
            
            // 扩展q1
            if (expand(q1, q2)) return step;
        }
        
        return -1;
    }
};


