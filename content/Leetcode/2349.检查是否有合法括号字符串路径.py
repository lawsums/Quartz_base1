from functools import cache
from typing import List


class Solution:
    def hasValidPath(self, grid: List[List[str]]) -> bool:
        m, n = len(grid), len(grid[0])

        # 状态 (x, y, c)：x 是列、y 是行，c 是从 (x, y) 出发时「已经攒下的余额」
        @cache
        def dfs(x: int, y: int, c: int) -> bool:
            if c < 0: return False # 如果有闭合不了的部分直接返回false
            if x >= n or y >= m: return False
            if x == n - 1 and  y == m - 1:
                # 到达终点时还没消费终点格子本身，所以要「余额 1 + 终点为 ')'」才归零
                if c == 1 and grid[y][x] == ')': return True
                return False

            # 消费当前格子，再走右边或下边
            new_c = c + int(grid[y][x] == '(') - int(grid[y][x] == ')')
            return dfs(x, y + 1, new_c) or dfs(x + 1, y, new_c)

        return dfs(0, 0, 0)
