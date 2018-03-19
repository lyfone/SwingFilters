//
// Created by zzhfeng on 2018/3/19.
//

#ifndef SWING_FILTERS_LINE_H
#define SWING_FILTERS_LINE_H

#pragma once

#include "MyPoint.h"

/**
 * 线结构
 */
struct Line {
    //直线起点
    MyPoint p;
    //直线斜率
    float k;
    //直线是否有效
    bool valid;

    Line();

    float get_point_by_index(int index);

    void update_start(MyPoint start);

    void update_k_by_point(MyPoint point);

    void update_k(float lk);
};

/**
 * 初始化
 */
Line::Line() {
    valid = false;
}

/**
 * 根据横坐标读取线在point的值
 * @param index
 * @return
 */
float Line::get_point_by_index(int index) {
    return p.point + k * (index - p.index);
}

/**
 * 更新线的起点
 * @param p
 */
void Line::update_start(MyPoint start) {
    p = start;
}

/**
 * 根据终点更新线的斜率
 * @param p
 */
void Line::update_k_by_point(MyPoint point) {
    k = (point.point - p.point) / (point.index - p.index);
    valid = true;
}

/**
 * 直接更新斜率
 * @param lk
 */
void Line::update_k(float lk) {
    k = lk;
    valid = true;
}

#endif //SWING_FILTERS_LINE_H
