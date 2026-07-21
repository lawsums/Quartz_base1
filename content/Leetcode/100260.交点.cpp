#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

class Solution {
public:
    // 浮点数判相等：替代 ==，避免double精度误差
    bool eq(double a, double b) { return fabs(a - b) < 1e-8; }
    // 浮点数小于等于：替代 <=
    bool le(double a, double b) { return a < b + 1e-8; }
    // 浮点数大于等于：替代 >=
    bool ge(double a, double b) { return a > b - 1e-8; }

    // 计算两条线段的交点，有交点返回坐标，无交点返回空
    vector<double> intersection(vector<int>& start1, vector<int>& end1, vector<int>& start2, vector<int>& end2) {
        // 全部转double，避免int整数除法丢精度
        double x1 = start1[0], y1 = start1[1];
        double x2 = end1[0],   y2 = end1[1];
        double x3 = start2[0], y3 = start2[1];
        double x4 = end2[0],   y4 = end2[1];
        double k1, k2, ans_x, ans_y;

        // ========== 情况1：两条都是竖直线 x=常数 ==========
        if (eq(x1, x2) && eq(x3, x4)) {
            // x不相等，两条竖线平行无交点
            if (!eq(x1, x3)) return {};
            // x相等，共线，判断y区间是否有重叠
            auto [miny1, maxy1] = minmax(y1, y2);
            auto [miny2, maxy2] = minmax(y3, y4);
            double y_low = max(miny1, miny2);   // 重叠区间下边界
            double y_high = min(maxy1, maxy2);  // 重叠区间上边界
            // 下边界 >= 上边界：无重叠
            if (ge(y_low, y_high)) return {};
            // 有重叠，返回任意一个交点
            return {x1, y_low};
        }

        // ========== 情况2：第一条是竖直线 x=x1 ==========
        if (eq(x1, x2)) {
            // 求第二条直线斜率
            k2 = (y3 - y4) / (x3 - x4);
            // 代入x=x1，求交点y坐标
            ans_y = k2 * (x1 - x3) + y3;
            // 判断交点y是否在两条线段y范围内
            bool on1 = ge(ans_y, min(y1,y2)) && le(ans_y, max(y1,y2));
            bool on2 = ge(ans_y, min(y3,y4)) && le(ans_y, max(y3,y4));
            if (on1 && on2) return {x1, ans_y};
            return {};
        }

        // ========== 情况3：第二条是竖直线 x=x3 ==========
        if (eq(x3, x4)) {
            // 求第一条直线斜率
            k1 = (y1 - y2) / (x1 - x2);
            // 代入x=x3，求交点y坐标
            ans_y = k1 * (x3 - x1) + y1;
            // 判断交点y是否在两条线段y范围内
            bool on1 = ge(ans_y, min(y1,y2)) && le(ans_y, max(y1,y2));
            bool on2 = ge(ans_y, min(y3,y4)) && le(ans_y, max(y3,y4));
            if (on1 && on2) return {x3, ans_y};
            return {};
        }

        // ========== 情况4：两条都不是竖直线，都有斜率 ==========
        // 计算两条直线斜率
        k1 = (y1 - y2) / (x1 - x2);
        k2 = (y3 - y4) / (x3 - x4);

        // 斜率相等：平行或共线
        if (eq(k1, k2)) {
            // 直线截距 y = kx + b
            double c1 = y1 - k1 * x1;
            double c2 = y3 - k2 * x3;
            // 截距不等：平行不共线，无交点
            if (!eq(c1, c2)) return {};
            // 截距相等：共线，判断x区间是否重叠
            auto [minx1, maxx1] = minmax(x1, x2);
            auto [minx2, maxx2] = minmax(x3, x4);
            double x_low = max(minx1, minx2);   // x重叠下边界
            double x_high = min(maxx1, maxx2);  // x重叠上边界
            // 无重叠
            if (ge(x_low, x_high)) return {};
            // 有重叠，返回交点
            return {x_low, k1 * x_low + c1};
        }

        // ========== 情况5：不平行，求直线交点 ==========
        // 联立直线方程求解交点x
        ans_x = ((k1 * x1 - k2 * x3) - (y1 - y3)) / (k1 - k2);
        // 代入求交点y
        ans_y = k1 * (ans_x - x1) + y1;

        // 检查交点x是否分别在两条线段x区间内
        auto [min1, max1] = minmax(x1, x2);
        auto [min2, max2] = minmax(x3, x4);
        bool inX1 = ge(ans_x, min1) && le(ans_x, max1);
        bool inX2 = ge(ans_x, min2) && le(ans_x, max2);

        // 检查交点y是否分别在两条线段y区间内
        auto [miny1, maxy1] = minmax(y1, y2);
        auto [miny2, maxy2] = minmax(y3, y4);
        bool inY1 = ge(ans_y, miny1) && le(ans_y, maxy1);
        bool inY2 = ge(ans_y, miny2) && le(ans_y, maxy2);

        // X、Y都在线段范围内，才是合法线段交点
        if (inX1 && inX2 && inY1 && inY2) return {ans_x, ans_y};
        return {};
    }
};
