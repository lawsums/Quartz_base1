package main

import "math"

func minWindow(s string, t string) string {
	cnt := map[rune]int{}
	depts := len(t)
	min_len := math.MaxInt
	start_id := -1

	for _, c := range t {
		cnt[c]++
	}

	l := 0
	for r, rc := range s {
		if cnt[rc] > 0 {
			depts--
		}
		// 减少频次, 最后会加回来所以没关系
		cnt[rc]--

		for depts == 0 {
			// 更新答案
			cur_len := r - l + 1
			if cur_len < min_len {
				min_len = cur_len
				start_id = l
			}

			var lc rune
			lc = s[l]
			// 等于零说明需要加回来, <0说明是无关字符
			if cnt[lc] == 0 {
				depts++
			}
			cnt[lc]++
			l++
		}
	}

	if start_id == -1 {
		return ""
	} else {
		return s[start_id : start_id+min_len]
	}
}
