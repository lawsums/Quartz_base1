#include <bits/stdc++.h>
#include <queue>
using namespace std;

class Solution {
public:
    int lastStoneWeight(vector<int>& stones) {
        priority_queue<int, vector<int>, less<>> pq; // 最大堆
        for (auto &stone : stones) {
            pq.push(stone);
        }

        while (pq.size() > 1) {
            int x = pq.top(); pq.pop();
            int y = pq.top(); pq.pop();
            if (x != y) pq.push(x - y); // 如果还有残余的话就加入
        }

        return pq.empty() ? 0 : pq.top();
    }
};
