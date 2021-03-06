//
// Created by zzhfeng on 2018/3/19.
//

#ifndef SWING_FILTERS_SWINGFILTERS_H
#define SWING_FILTERS_SWINGFILTERS_H

#pragma once

#include "Line.h"
#include <math.h>

/**
 * 数据点之于拟合直线的状态表
 */
enum POINT_STATE {
    BELOW_U_MORE_THAN_E,    //低于上直线U且超过偏差范围
    ABOVE_L_MORE_THAN_E,    //高于下直线L且超过偏差范围
    BOTH_MORE_THAN_E,       //两者都超过偏差范围
    ACCEPT,                 //数据点在范围内,可以拟合
    REJECT,                 //数据点在范围外,拒绝拟合
};

/**
 * swing filters存储结构
 */
struct SF_DATA {
    //上拟合线
    Line u;
    //下拟合线
    Line l;
    //拟合直线
    Line fitting_line;
};

/**
 * Swing_Filters类
 */
class Swing_Filters {

private:
    //拟合过程直线的数据结构
    SF_DATA sf_data;
    //偏差范围
    float deviation_tolerance;

    //拟合直线参数计算的中间值
    float aig_dividend; //被除数
    float aig_divisor;  //除数

    //判断数据点对于拟合直线的状态
    POINT_STATE check_point(MyPoint p);

    //获取最终拟合直线的参数
    float get_a();

    //更新中间结果
    void update_Aig(MyPoint p);

public:
    //初始化
    Swing_Filters(float e);

    //开始拟合
    void start(MyPoint p1, MyPoint p2, bool b);

    //新的数据点到达
    bool push_point(MyPoint p);

    //读取拟合结果
    Line get_res_line();

    //重置
    void reset();
};

/**
 * 构造函数
 * @param e
 */
Swing_Filters::Swing_Filters(float e) {
    deviation_tolerance = e;
}

/**
 * 开始进行拟合
 * @param p1    拟合起点p1
 * @param p2    拟合起点p2
 * @param b     当为true时表示起点p1为数据点
 *              当为false时表示起点p1为上一拟合线段终点
 */
void Swing_Filters::start(MyPoint p1, MyPoint p2, bool b) {
    sf_data.u.update_start(p1);
    sf_data.l.update_start(p1);
    sf_data.fitting_line.update_start(p1);
    sf_data.fitting_line.inc_len();
    if(b) sf_data.fitting_line.start_valid = true;

    MyPoint temp = p2;

    temp.point += deviation_tolerance;
    sf_data.u.update_k_by_point(temp);

    temp.point -= deviation_tolerance * 2;
    sf_data.l.update_k_by_point(temp);
}

/**
 * 根据新到达的数据点计算拟合情况
 * @param p
 * @return 返回结果为true时表示数据点可以继续拟合，
 *          返回结果为false时表示依次拟合结束，应该开始新的拟合
 */
bool Swing_Filters::push_point(MyPoint p) {
    POINT_STATE p_state = check_point(p);
    switch (p_state) {
        case BOTH_MORE_THAN_E: {
            MyPoint tmp = p;
            tmp.point += deviation_tolerance;
            sf_data.u.update_k_by_point(tmp);
            tmp.point -= deviation_tolerance * 2;
            sf_data.l.update_k_by_point(tmp);
            break;
        }
        case BELOW_U_MORE_THAN_E: {
            MyPoint tmp = p;
            tmp.point += deviation_tolerance;
            sf_data.u.update_k_by_point(tmp);
            break;
        }
        case ABOVE_L_MORE_THAN_E: {
            MyPoint tmp = p;
            tmp.point -= deviation_tolerance;
            sf_data.l.update_k_by_point(tmp);
            break;
        }
        case REJECT: {
            float para = get_a();
            sf_data.fitting_line.update_k(para);
            return false;
        }
        case ACCEPT:
            break;
        default:
            break;
    }
    sf_data.fitting_line.inc_len();
    update_Aig(p);
    return true;
}

/**
 * 判断点的偏离状态
 * @param p
 * @return
 */
POINT_STATE Swing_Filters::check_point(MyPoint p) {
    float y1 = sf_data.u.get_point_by_index(p.index);
    float y2 = sf_data.l.get_point_by_index(p.index);
    if ((y1 + deviation_tolerance < p.point) || (y2 - deviation_tolerance > p.point))
        return REJECT;
    if ((y1 - deviation_tolerance > p.point) && (y2 + deviation_tolerance < p.point))
        return BOTH_MORE_THAN_E;
    if (y1 - deviation_tolerance > p.point)
        return BELOW_U_MORE_THAN_E;
    if (y2 + deviation_tolerance < p.point)
        return ABOVE_L_MORE_THAN_E;
    return ACCEPT;
}

/**
 * 获取最终拟合直线的斜率
 * @return
 */
float Swing_Filters::get_a() {
    float aig = aig_dividend / aig_divisor;
    float aiu = sf_data.u.k;
    float ail = sf_data.l.k;
    return fmax(fmin(aig, aiu), ail);
}

/**
 * 根据新的数据点更新最终拟合直线中间计算结果的斜率
 * @param p
 */
void Swing_Filters::update_Aig(MyPoint p) {
    aig_dividend += (p.point - sf_data.fitting_line.p.point) * (p.index - sf_data.fitting_line.p.index);
    aig_divisor += (p.index - sf_data.fitting_line.p.index) * (p.index - sf_data.fitting_line.p.index);
}

/**
 * 读取拟合结果
 * @return
 */
Line Swing_Filters::get_res_line() {
    if (sf_data.fitting_line.valid)
        return sf_data.fitting_line;
    else {
        //TODO 如果拟合线段不存在，即还没有求解出来，则不可读取拟合结果
    }
}

/**
 * 重置
 */
void Swing_Filters::reset() {
    sf_data.fitting_line = Line();
    sf_data.l = sf_data.u = Line();
    aig_divisor = 0.0;
    aig_dividend = 0.0;
}

#endif //SWING_FILTERS_SWINGFILTERS_H
