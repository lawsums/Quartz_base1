#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

class Solution {
private:
    vector<vector<string>> ans;
    vector<string> path;
    int n;
    // vector<bool> rows;
    vector<bool> cols;
    vector<bool> diags;
    vector<bool> rdiags;

public:
    vector<vector<string>> solveNQueens(int n) {
        init(n);
        dfs(0);
        return ans;
    }

    void dfs(int i) {
        // 如果深度到达n
        if (i == n) {
            ans.push_back(path);
            return;
        }

        for (int j = 0; j < n; ++j) {
            // 如果满足条件的话就放置Q
            if (legal(i, j)) {
                pin(i, j);
                dfs(i + 1);
                unpin(i, j);
            }
        }
    }

private:
    void init(int n) {
        this->n = n;
        path.assign(n, string(n, '.'));
        // rows.resize(n);
        cols.resize(n);
        diags.resize(2 * n - 1);
        rdiags.resize(2 * n - 1);
    }

    void pin(int i, int j) {
        path[i][j] = 'Q';
        // rows[i] = true;
        cols[j] = true;
        diags[i + j] = true;         // [0, 2*n-1)
        rdiags[i - j + n - 1] = true;// i - j ~ [-n+1, n)
    }

    void unpin(int i, int j) {
        path[i][j] = '.';
        // rows[i] = false;
        cols[j] = false;
        diags[i + j] = false;         // [0, 2*n-1)
        rdiags[i - j + n - 1] = false;// i - j ~ [-n+1, n)
    }

    bool legal(int i, int j) {
        if (cols[j] || diags[i + j] || rdiags[i - j + n - 1]) {
            return false;
        }
        return true;
    }
};

