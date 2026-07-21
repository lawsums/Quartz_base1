#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int countStudents(vector<int>& students, vector<int>& sandwiches) {
        queue<int> q;
        for (const auto &student : students) {
            q.push(student);
        }

        stack<int> s;
        for (int i = sandwiches.size() - 1; i >= 0; i--) {
            s.push(sandwiches[i]);
        }

        // 模拟就行
        int i = 0;
        while (!q.empty()) {
            for (i = 0; i < q.size() && q.front() != s.top(); i++) {
                q.push(q.front()); 
                q.pop(); // 转移回去
            } 
            if (i == q.size()) return q.size();
            // 弹出三明治和学生 
            s.pop(); 
            q.pop();
        }

        return q.size();
    }
};

