class Solution:
    def findNthDigit(self, n: int) -> int:
        pows = [pow(10, i) for i in range(12)]
        pre = []

        # pre[i] = 前 i+1 段累计占用的位数
        for i in range(11):
            # (i+1) 位数的范围是 [10^i, 10^(i+1) - 1]，共 9 * 10^i 个
            start = pows[i] + 1
            end = pows[i + 1]
            count = end - start + 1
            total_sum = (i + 1) * count  # 每数占 (i+1) 位，O(1) 复杂度

            if i == 0:
                pre.append(total_sum)
            else:
                pre.append(pre[-1] + total_sum)

        idx = bisect_left(pre, n)

        # 确定数字
        num = pows[idx] + (n - (pre[idx - 1] if idx > 0 else 0) - 1) // (idx + 1)
        offset = (n - pre[idx - 1] - 1) % (idx + 1)
        return int(str(num)[offset])
