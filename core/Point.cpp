//
// Created by HB on 5/19/2015.
//

#include "Point.h"
#include "Math.h"

Point::Point() {
    x = 0;
    y = 0;
}

Point::Point(double x, double y)
    : x(x)
    , y(y)
{
}

double Point::getX() {
    return x;
}

double Point::getY() {
    return y;
}

double Point::distance(const Point &p) {
    return sqrt(
            (x - p.x) * (x - p.x)
            +
            (y - p.y) * (y - p.y));
}

double Point::distanceSq(const Point &p) {
    return ((x - p.x) * (x - p.x)
            +
            (y - p.y) * (y - p.y));
}

void Point::setPerpendicular(const Point &p) {
    x = -p.y;
    y = p.x;
}

void Point::setPerpendicular(const Point &p1, const Point &p2) {
    x = p1.x + p2.x;
    y = p1.y + p2.y;

    double a = p2.x * y - p2.y * x;

    if (a < 0.01 && a > -0.01) {
        /* Almost straight */
        x = -p2.y;
        y = p2.x;
    } else {
        x /= a;
        y /= a;
    }
}
