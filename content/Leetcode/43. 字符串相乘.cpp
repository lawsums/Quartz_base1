#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
  // 功能：实现两个非负整数字符串的乘法（避免整数溢出，适用于超大数）
  // 参数：num1 - 第一个整数的字符串形式，num2 - 第二个整数的字符串形式
  // 返回值：两个数相乘结果的字符串形式
  string multiply(string num1, string num2) {
    // 边界条件：任一数为"0"，乘积直接返回"0"（避免后续无效计算）
    if (num1 == "0" || num2 == "0")
      return "0";

    // 反转字符串：将数字从"高位在前"转为"低位在前"，方便按位相乘（从个位开始计算）
    reverse(num1.begin(), num1.end());
    reverse(num2.begin(), num2.end());

    int n = num1.size(); // 第一个数的位数（反转后仍为原位数）
    int m = num2.size(); // 第二个数的位数
    // ans数组：存储每一位的乘积和+进位结果，初始化为0
    // 大小设为n+m+1：两个数相乘的最大位数为n+m（如999*999=998001，3+3=6位），+1预留最终进位空间
    vector<int> ans(n + m + 1, 0);

    // 第一步：计算所有位的乘积和（不立即处理进位，先累加对应位的乘积）
    // 原理：num1的第i位（原个位为i=0）与num2的第j位相乘，结果落在最终结果的第i+j位
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        // num1[i]-'0'：将字符形式的数字转为整数
        // 乘积累加到ans[i+j]：同一位置的所有乘积求和（后续统一处理进位）
        ans[i + j] += (num1[i] - '0') * (num2[j] - '0');
      }
    }

    // 第二步：统一处理所有位的进位（从低位到高位）
    int carry = 0; // 记录当前位向高位的进位值
    for (int i = 0; i < ans.size(); i++) {
      ans[i] += carry;      // 加上上一位传递过来的进位
      carry = ans[i] / 10;  // 计算当前位的新进位（除以10取商）
      ans[i] = ans[i] % 10; // 保留当前位的有效数字（除以10取余）
    }

    // 第三步：将ans数组的整数转为字符（0->'0'，1->'1'...）
    for (int i = 0; i < ans.size(); i++) {
      ans[i] += '0'; // 利用ASCII码偏移：'0'的ASCII是48，整数+48即为对应字符
    }

    // 第四步：反转ans数组，恢复"高位在前"的正常数字顺序
    // （ans原本是低位在前，反转后变为高位在前，如[8,0,8,6,5]→"56808"）
    string res = string(ans.rbegin(), ans.rend());

    // 第五步：去除结果字符串的前导零（反转后可能存在多余的前导零）
    int idx = 0;
    while (idx < res.size() && res[idx] == '0') {
      idx++; // 找到第一个非零字符的位置
    }

    // 返回去除前导零后的结果（idx不会等于res.size()，因为前面已处理0的情况）
    return res.substr(idx);
  }
};
