// 我的做法
class Solution {
public:
    int myAtoi(string s) {
        long long ans = 0;
        char pre = 0;
        bool neg = false;
        for (const auto &c : s) {
            if (!isdigit(pre) && pre != '+' && pre != '-') {
                if (c == ' ' || c == '+') ;
                else if (c == '-') neg = true;
                else if (isdigit(c)) { 
                    ans = 10 * ans + (c - '0'); 
                    if (ans > INT_MAX || ans < INT_MIN) break;
                }
                else break;
            } else {
                if (isdigit(c)) { 
                    ans = 10 * ans + (c - '0'); 
                    if (ans > INT_MAX || ans < INT_MIN) break;
                }
                else break;
            }
            pre = c;
        }

        ans = neg ? -ans : ans;
        if (ans > INT_MAX) {
            return INT_MAX;
        } else if (ans < INT_MIN) {
            return INT_MIN;
        } else {
            return ans;
        }
    }
};


// 精简做法
class Solution {
public:
    int myAtoi(string s) {
        int n = s.size();
        int i = 0;
        int sign = 1;
        long long ans = 0;

        // 跳过前导空格
        while (i < n && s[i] == ' ') i++;

        // 处理正负号
        if (i < n && (s[i] == '+' || s[i] == '-')) {
            sign = (s[i++] == '+') ? 1 : -1;
        }

        // 读取数字
        while (i < n && isdigit(s[i])) {
            ans = ans * 10 + (s[i++] - '0');
            // 溢出判断
            if (ans > INT_MAX) {
                return sign == 1 ? INT_MAX : INT_MIN;
            }
        }

        return ans * sign;
    }
};
