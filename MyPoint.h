//
// Created by zzhfeng on 2018/3/19.
//

#ifndef SWING_FILTERS_POINT_H
#define SWING_FILTERS_POINT_H

#pragma once

struct MyPoint {
    int index;
    float point;

    MyPoint();

    MyPoint(int i, float p);
};

MyPoint::MyPoint() {

}

MyPoint::MyPoint(int i, float p) {
    index = i;
    point = p;
}


#endif //SWING_FILTERS_POINT_H
