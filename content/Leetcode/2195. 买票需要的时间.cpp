#include <bits/stdc++.h>
using namespace std;

typedef array<int ,2> aii;

class Solution {
public:
    int timeRequiredToBuy(vector<int>& tickets, int k) {
        queue<aii> q;
        for (int i = 0; i < tickets.size(); i++) {
            q.push({tickets[i], i}); // 加入[现在需要的票数, 序号]
        }

        int cnt = 0;
        while(true) {
            aii cur = q.front(); q.pop();
            if (--cur[0] > 0) { // 对于当前排到的人的票数-1, 如果没有满足还要加回去
                q.push(cur); 
            } else { // 已经满足了
                if (cur[1] == k) return cnt;
            }
            cnt++;
        }

        return -1;
    }
};
