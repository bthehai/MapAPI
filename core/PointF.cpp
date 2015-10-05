//
// Created by HB on 5/19/2015.
//

#include "PointF.h"


PointF::PointF() {
    x = 0;
    y = 0;
}

PointF::PointF(float x, float y)
    : x(x)
    , y(y)
{
}

float PointF::getX() {
    return x;
}

float PointF::getY() {
    return y;
}
