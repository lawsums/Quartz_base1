#include <bits/stdc++.h>
#include <cctype>
using namespace std;

// 使用原始c++11方式
class Solution1 {
public:
    string maskPII(string s) {
        size_t atPos = s.find('@');
        if (atPos != string::npos) {
            return maskEmail(s, atPos); // 处理邮箱的逻辑
        } else {
            return maskPhone(s);
        }
    }

private:
    string maskEmail(string &s, size_t atPos) {
        // 名字 和 域名 部分的大写英文字母应当转换成小写英文字母。w
        for (auto &c : s) {
            if (isalpha(c)) {
                c = tolower(c); // 转成大写
            }
        }

        // 名字 中间的字母（即，除第一个和最后一个字母外）必须用 5 个 "*****" 替换。
        // 用string(1, c)生成单字符字符串，替代错误的to_string(c)
    string prefix = string(1, s[0]) + string(5, '*') + string(1, s[atPos - 1]);

        // 返回prefix + 转换成小写之后的后缀部分
        return prefix + s.substr(atPos); 
    }

    string maskPhone(string &s) {
        // 统计数字个数
        vector<char> nums;
        for (int i = 0; i < s.size(); i++) {
            if ('0' <= s[i] && s[i] <= '9') {
                nums.push_back(s[i]);
            }
        }
        int n = nums.size();

        string ans = "***-***-" + string(nums.end() - 4, nums.end()); 

        n -= 10;
        if (n) {
            ans = "+" + string(n, '*') + "-" + ans;
        }

        return ans;
    }
};


#include <format> // 必须包含format头文件（C++20+）

// C++20 format格式(类似于python f-string)
class Solution2 {
public:
    string maskPII(string s) {
        size_t atPos = s.find('@');
        return atPos != string::npos ? maskEmail(s, atPos) : maskPhone(s);
    }

private:
    string maskEmail(string s, size_t atPos) {
        // 字母转小写
        transform(s.begin(), s.end(), s.begin(), [](char c) {
            return isalpha(c) ? tolower(c) : c;
        });
        // 直接format，一步拼接（和Python f-string一样爽）
        return format("{}*****{}{}", s[0], s[atPos-1], s.substr(atPos));
    }

    string maskPhone(string s) {
        // 提取所有数字（用erase+remove_if，更简洁的C++写法）
        s.erase(remove_if(s.begin(), s.end(), [](char c) {
            return !isdigit(c);
        }), s.end());
        int country = s.size() - 10;
        string last4 = s.substr(s.size() - 4);
        // 按需format，分情况拼接（无冗余代码）
        if (country == 0) {
            return format("***-***-{}", last4);
        } else {
            return format("+{}-***-***-{}", string(country, '*'), last4);
        }
    }
};

// 测试
int main() {
    Solution2 sol;
    cout << sol.maskPII("LeetCode@LeetCode.com") << endl; // l*****e@leetcode.com
    cout << sol.maskPII("1(234)567-890") << endl; // ***-***-7890
    cout << sol.maskPII("+86 12345678901") << endl; // +**-***-***-8901
    return 0;
}
