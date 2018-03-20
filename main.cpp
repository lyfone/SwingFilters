#include <iostream>
#include "SwingFilters.h"
#include <vector>

int main() {
    std::cout << "Hello, World!" << std::endl;

    int start = false;
    int counter = 2;
    Swing_Filters swing_filters(1);
    float f;
    MyPoint point;
    MyPoint start_point(1, 3.0);
    std::vector<Line> res_line;
    bool star_flag = true;

    while (true) {
        std::cout << "输入数值：";
        std::cin >> f;
        //输入0结束测试
        if (f == 0) break;
        point.point = f;
        point.index = counter;
        counter++;
        if (!start) {
            swing_filters.start(start_point, point, star_flag);
            start = true;
        } else {
            bool state = swing_filters.push_point(point);
            if (state) continue;
            else {
                Line res = swing_filters.get_res_line();
                res_line.push_back(res);
                start = false;
                start_point.point = res.len * res.k + res.p.point;
                swing_filters.reset();
                star_flag = false;
                counter = 2;
                std::cout << "一次线段拟合完成！" << std::endl;
                std::cout << "len : " << res.len << std::endl;
            }
        }
    }
    return 0;
}