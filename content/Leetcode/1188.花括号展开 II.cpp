#include <bits/stdc++.h>
using namespace std;

// 需要 C++23（this auto&& self / span / views::drop）
class Solution {
public:
    static vector<string> braceExpansionII(const string& s) {
        return [str = s.data()](this auto&& self) {
            vector<string> result, now;   // result: 本层的并集; now: 当前项的乘积集合
            now.emplace_back();           // 乘法的单位元 ε = ""（空集初值会让结果恒为 ∅）
            for (;;) {
                const auto ch = *str++;
                if (ch == '}' or ch == 0) {          // '}' 或字符串结尾 '\0'：本层收尾
                    result.reserve(result.size() + now.size());
                    for (auto &s: now) {
                        result.push_back(move(s));
                    }
                    sort(result.begin(), result.end());
                    result.resize(unique(result.begin(), result.end()) - result.begin());  // 每层去重
                    return result;
                }
                else if (ch == '{') {
                    auto vec = self();           // 递归解析子表达式，得到子集合
                    now.reserve(now.size() * vec.size());  // 必须先 reserve，否则 push_back 扩容会让 ori 悬垂
                    const span<string> ori = now;          // 只观察前 now.size() 个元素，不拷贝
                    for (auto &s: vec | views::drop(1)) {  // 与 vec[1..] 相乘：无处复用，只能新建
                        for (auto &xs: ori) {
                            now.push_back(xs + s);
                        }
                    }
                    for (auto &sf = vec.front(); auto &s: ori) {  // 与 vec[0] 相乘：就地追加，省一次拷贝
                        s += sf;
                    }
                }
                else if (ch == ',') {                // 逗号：当前项收尾并入 result，并重置 now = {""}
                    result.reserve(result.size() + now.size());
                    for (auto &s: now) {
                        result.push_back(move(s));
                    }
                    now.clear();
                    now.emplace_back();
                }
                else {                               // 普通字母：now × {ch}
                    for (auto &s: now) {
                        s.push_back(ch);
                    }
                }
            }
        } ();
    }
};
