#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Solution {
private:
    vector<int> parent;  // 并查集父节点数组
    vector<int> rank;    // 按秩合并的辅助数组

    // 查找根节点（带路径压缩）
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    // 合并两个节点（带按秩合并）
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return;
        // 秩小的树合并到秩大的树
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else {
            parent[rootY] = rootX;
            if (rank[rootX] == rank[rootY]) {
                rank[rootX]++;
            }
        }
    }

    // 判断两个字符串是否相似
    bool isSimilar(const string& a, const string& b) {
        if (a == b) return true;
        vector<int> diff;  // 记录不同字符的位置
        for (int i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) {
                diff.push_back(i);
                if (diff.size() > 2) return false;  // 不同位置超过2个，直接不相似
            }
        }
        // 必须恰好两个不同位置，且交换后相等
        return diff.size() == 2 && a[diff[0]] == b[diff[1]] && a[diff[1]] == b[diff[0]];
    }

public:
    int numSimilarGroups(vector<string>& strs) {
        int n = strs.size();
        // 初始化并查集
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }

        // 遍历所有字符串对，相似则合并
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (isSimilar(strs[i], strs[j])) {
                    unite(i, j);
                }
            }
        }

        // 统计连通分量个数（根节点的数量）
        int count = 0;
        for (int i = 0; i < n; i++) {
            if (find(i) == i) {
                count++;
            }
        }
        return count;
    }
};
