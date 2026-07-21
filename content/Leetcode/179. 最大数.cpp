// 我的方法
class Solution {
public:
    string largestNumber(vector<int>& nums) {
        // 字典序问题, 这个比大小是通过拼接之后看字符串的大小比较的
        sort(nums.begin(), nums.end(), [](int a, int b) {
            string s1 = to_string(a) + to_string(b);
            string s2 = to_string(b) + to_string(a);
            return s1 > s2; // 降序排序
                });

        string ans;
        for (const auto& num : nums) {
            ans += to_string(num);
        }
        
        return all_zero(nums) ? "0" : ans;
    }

    bool all_zero(vector<int> &nums) {
        for (const auto& num : nums) {
            if (num != 0) return false;
        }
        return true;
    }
};

// 更规范的写法
class Solution {
public:
    string largestNumber(vector<int>& nums) {
        // 如果全是 0，直接返回 "0"
        if (all_of(nums.begin(), nums.end(), [](int x) { return x == 0; })) {
            return "0";
        }

        // 转成字符串数组
        vector<string> strs;
        strs.reserve(nums.size());
        for (int x : nums) {
            strs.push_back(to_string(x));
        }

        // 排序规则：a + b > b + a
        sort(strs.begin(), strs.end(), [](const string& a, const string& b) {
            return a + b > b + a;
        });

        // 合并
        return accumulate(strs.begin(), strs.end(), string(),
                          [](const string& a, const string& b) {
                              return a + b;
                          });
    }
};

int main() {
    Solution s;
    vector<int> nums = {3, 30, 34, 5, 9};
    cout << s.largestNumber(nums) << "\n"; // 输出 9534330
}
