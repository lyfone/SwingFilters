//
// Created by zzhfeng on 2018/3/19.
//

#ifndef SWING_FILTERS_POINT_H
#define SWING_FILTERS_POINT_H

#pragma once

struct MyPoint{
    int index;
    float point;
    bool valid;

    MyPoint();
};

MyPoint::MyPoint() {
    valid = false;
}

#endif //SWING_FILTERS_POINT_H
