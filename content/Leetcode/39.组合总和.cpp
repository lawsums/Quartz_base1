// #include <bits/stdc++.h>
#include <vector>
using namespace std;

class Solution {
private:
    int target;
    vector<int> path;
    vector<vector<int>> ans;

public:
    vector<vector<int>> combinationSum(vector<int> &candidates, int target) {
        this->target = target;
        dfs(0, 0, candidates);
        return ans;
    }

    void dfs(int begin, int sum, vector<int> &candidates) {
        if (sum >= target) {
            if (sum == target) ans.push_back(path);
            return;
        }

        // printf("%d->", begin);
        // for (const auto& elem : path) printf("\t%d, ", elem);
        // cout << endl;

        for (int i = begin; i < candidates.size(); i++) {
            path.push_back(candidates[i]);
            dfs(i, sum + candidates[i], candidates);
            path.pop_back();
        }
    }
};

int main() {
    vector<int> candidates = {2, 3, 6, 7};
    int target = 7;
    Solution().combinationSum(candidates, target);
}