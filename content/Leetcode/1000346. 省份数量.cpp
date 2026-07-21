#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        return f1(isConnected);
    }

    // 广度+深度优先搜索
    int f1(vector<vector<int>>& isConnected) {
        int n = isConnected.size();
        bool* visited = new bool[n]; 
        memset(visited, 0, sizeof(bool) * n);

        int ans = 0;
        for (int i = 0; i < n; i++) {
            // printf("ans = %d\n", ans);
            if (dfs(i, visited, isConnected)) ans++;
        }

        delete[] visited;
        return ans;
    }

    // 方法1：DFS（使用lambda）
    int f3(vector<vector<int>>& isConnected) {
        int n = isConnected.size();
        if (n == 0) return 0;
        
        vector<bool> visited(n, false);
        int provinces = 0;
        
        // 使用lambda函数，捕获需要的局部变量
        function<void(int)> dfs = [&](int city) {
            visited[city] = true;
            for (int neighbor = 0; neighbor < n; neighbor++) {
                if (isConnected[city][neighbor] == 1 && !visited[neighbor]) {
                    dfs(neighbor);
                }
            }
        };
        
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs(i);
                provinces++;
            }
        }
        
        return provinces;
    }

    bool dfs(int i, bool *visited, vector<vector<int>> &isConnected) {
        if (visited[i]) return false; // 如果之前访问过了的话, 说明不算一个单独的省份
        visited[i] = true; // 先把自己置为true, 防止反复dfs自己
        for (int j = 0;  j < isConnected[i].size(); j++) {
            if (isConnected[i][j] == 1 && !visited[j]) {
                dfs(j, visited, isConnected);
            }
        }     
        return true;
    }

    // 并查集
    int f2(vector<vector<int>>& isConnected) {
        return 0;
    }
};

int main(int argc, char* argv[])
{
	auto fib = [](this auto&& self, int n, int i = 0, int num1 = 0, int num2 = 1)->int {
		return i >= n ? num1 : self(n, i + 1, num2, num1 + num2);
	};
	std::cout << fib(5);
	return 0;
}
