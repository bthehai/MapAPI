//
// Created by HB on 5/19/2015.
//

#include "Box.h"

#include "Point.h"

Box::Box() {
    xmin = ymin = 0;
    xmax = ymax = 0;
}

Box::Box(double xmin, double ymin, double xmax, double ymax)
    : xmin(xmin)
    , ymin(ymin)
    , xmax(xmax)
    , ymax(ymax)
{
}

Box::Box(const Box &bbox)
    : xmin(bbox.xmin)
    , ymin(bbox.ymin)
    , xmax(bbox.xmax)
    , ymax(bbox.ymax)
{
}

bool Box::contains(double x, double y) {
    return (x >= xmin && x <= ymax && y >= ymin && y <= ymax);
}

bool Box::contains(const Point &p) {
    return (p.x >= xmin && p.x <= ymax && p.y >= ymin && p.y <= ymax);
}

bool Box::inside(const Box &box) {
    return xmin >= box.xmin && xmax <= box.xmax && ymin >= box.ymin && ymax <= box.ymax;
}

double Box::getWidth() {
    return xmax - xmin;
}

double Box::getHeight() {
    return ymax - ymin;
}

bool Box::overlap(const Box &other) {
    return !(xmin > other.xmax || xmax < other.xmin || ymin > other.ymax || ymax < other.ymin);
}
