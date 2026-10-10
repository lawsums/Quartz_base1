from functools import lru_cache


class Solution:
    def findGoodStrings(self, n: int, s1: str, s2: str, evil: str) -> int:
        MOD = 1000000007
        m = len(evil)
        e = evil

        # 构建 KMP 的 next 数组
        nxt = [-1] * (m + 1)
        if m > 1:
            nxt[1] = 0
        i = 2
        cn = 0
        while i < m:
            if e[i - 1] == e[cn]:
                cn += 1
                nxt[i] = cn
                i += 1
            elif cn > 0:
                cn = nxt[cn]
            else:
                nxt[i] = 0
                i += 1

        def jump(pick, j):
            # 当前字符是 pick，一开始匹配 e[j]
            # 根据 next 数组加速匹配，返回匹配出来的位置
            while j >= 0 and pick != e[j]:
                j = nxt[j]
            return j

        def kmp(s):
            # 判断 s 中是否包含 evil
            x = 0
            y = 0
            while x < n and y < m:
                if s[x] == e[y]:
                    x += 1
                    y += 1
                elif y == 0:
                    x += 1
                else:
                    y = nxt[y]
            return x - y if y == m else -1

        @lru_cache(maxsize=None)
        def f(s, i, j, free):
            # s: 上界字符串
            # i: 当前决策位置
            # j: 已经匹配了 e[0...j-1]
            # free: 1 表示之前的决策已经比 s 小了，0 表示和 s 的前缀一样
            if j == m:
                return 0
            if i == n:
                return 1
            cur = s[i]
            ans = 0
            if free == 0:
                # 之前的决策和 s 的状况一样
                # 当前尝试比 cur 小的字符
                for pick in range(ord('a'), ord(cur)):
                    ans = (ans + f(s, i + 1, jump(chr(pick), j) + 1, 1)) % MOD
                # 当前尝试等于 cur 的字符
                ans = (ans + f(s, i + 1, jump(cur, j) + 1, 0)) % MOD
            else:
                # 之前的决策已经确定小于 s 了，a~z 随便尝试
                for pick in range(ord('a'), ord('z') + 1):
                    ans = (ans + f(s, i + 1, jump(chr(pick), j) + 1, 1)) % MOD
            return ans

        # <= s2 的好字符串数量
        ans = f(s2, 0, 0, 0)
        # 减去 < s1 的好字符串数量，即 <= s1 的好字符串数量再减去 s1 本身（若 s1 是好字符串）
        ans = (ans - f(s1, 0, 0, 0) + MOD) % MOD
        # 如果 s1 本身不包含 evil，则 s1 也是好字符串，但前面减去了它，需要加回来
        if kmp(s1) == -1:
            ans = (ans + 1) % MOD
        return ans
