class Solution:
    def numberOfWays(self, s: str) -> int:
        n = len(s)
        # dp[k][j]: 处理前 j 个字符后，状态 k 的方案数
        # 状态: 0:"0"  1:"1"  2:"01"  3:"10"  4:"010"  5:"101"
        dp = [[0] * (n + 1) for _ in range(6)]

        for j in range(1, n + 1):
            # 先继承上一轮
            for k in range(6):
                dp[k][j] = dp[k][j - 1]

            c = s[j - 1]
            if c == '0':
                dp[0][j] += 1
                dp[3][j] += dp[1][j - 1]
                dp[4][j] += dp[2][j - 1]
            else:  # c == '1'
                dp[1][j] += 1
                dp[2][j] += dp[0][j - 1]
                dp[5][j] += dp[3][j - 1]

        return dp[4][n] + dp[5][n]
