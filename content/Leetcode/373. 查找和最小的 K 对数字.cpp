#include <bits/stdc++.h>
#include <queue>
using namespace std;

using ati = array<int, 3>;

class Solution {
public:
    vector<vector<int>> kSmallestPairs(vector<int>& nums1, vector<int>& nums2, int k) {
        return f1(nums1, nums2, k);
    }

    // 标准做法
    vector<vector<int>> f1(vector<int>& nums1, vector<int>& nums2, int k) {
        vector<vector<int>> ans;
        int m = nums1.size(), n = nums2.size();
        if (m == 0 || n == 0 || k == 0) return ans; // 边界条件：空数组/取0个
        
        // 最小堆：按数对和升序，存储{和, i, j}
        priority_queue<ati, vector<ati>, greater<>> pq;
        
        // 初始入堆：nums1的前min(k, m)个元素 + nums2[0]，避免入堆过多
        for (int i = 0; i < min(k, m); i++) {
            pq.push({nums1[i] + nums2[0], i, 0});
        }
        
        // 取k个最小数对，或堆为空时停止
        while (k-- > 0 && !pq.empty()) {
            auto cur = pq.top();
            pq.pop();
            int i = cur[1], j = cur[2];
            ans.push_back({nums1[i], nums2[j]});
            
            // 补充新数对：当前数对的nums2下标+1（j+1 < n 才入堆，避免越界）
            if (j + 1 < n) {
                pq.push({nums1[i] + nums2[j+1], i, j+1});
            }
        }
        
        return ans;
    }

    // 我的方法(超出内存限制)
    vector<vector<int>> f2(vector<int>& nums1, vector<int>& nums2, int k) {
        priority_queue<ati, vector<ati>, greater<>> pq; // 最小堆

        for (int i = 0; i < nums1.size(); i++) {
            for (int j = 0; j < nums2.size(); j++) {
                pq.push({nums1[i] + nums2[j], i, j}); // 通过最小堆排序
            }
        }

        vector<vector<int>> ans;
        while (k--) {
            ati cur = pq.top(); pq.pop();
            ans.push_back({nums1[cur[1]], nums2[cur[2]]});
        }

        return ans;
    }
};
