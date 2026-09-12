#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int totalNumbers(vector<int>& digits) {
        int n = digits.size();
        vector<bool> visit(n, false);
        unordered_set<int> s;
        int path = 0;

        function<void(int)> dfs = [&](int i) {
            if (i == min(3, n)) {
                if (i == 3 && path % 2 == 0) {
                    s.insert(path);
                }
                return;
            }

            // 回溯
            for (int j = 0; j < n; ++j) {
                if (visit[j]) continue;
                if (digits[j] == 0 && path == 0) continue;

                visit[j] = true;
                path = path * 10 + digits[j];

                dfs(i + 1);

                visit[j] = false;
                path /= 10;
            }
        };

        dfs(0);
        return (int)s.size();
    }
};
