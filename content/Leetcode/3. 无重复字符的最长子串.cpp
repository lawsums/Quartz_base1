class Solution {
public:
    // [3. 无重复字符的最长子串](https://leetcode.cn/problems/longest-substring-without-repeating-characters/)
    int lengthOfLongestSubstring(string s) {
        int n = s.size();
        vector<int> last(256, -1);
        int ans = 0;

        for (int l = 0, r = 0; r < n; r++) {
            l = max(l, last[s[r]] + 1); // 维护窗口左边界
            ans = max(ans, r - l + 1); // 更新答案
            last[s[r]] = r; // 更新上次出现位置
        }
        
        return ans;
    }

    // // 哈希表记录
    // int lengthOfLongestSubstring(string s) {
    //     map<char, int> mp;
    //     int ans = 0;
    //     for (int l = 0, r = 0; r < s.size(); r++) {
    //         const auto& c = s[r];
    //         while (mp.count(c) && mp[c] > 0) {
    //             mp[s[l++]]--;
    //         }
    //         mp[c]++;
    //         ans = max(ans, r - l + 1);
    //     }
    //     return ans;
    // }
};