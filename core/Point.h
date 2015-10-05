//
// Created by HB on 5/19/2015.
//

#ifndef VECTORTILEMAP_POINT_H
#define VECTORTILEMAP_POINT_H


class Point {
public:
    double x;
    double y;

public:
    Point();
    Point(double x, double y);

public:
    double getX();
    double getY();

    double distance(const Point &p);
    double distanceSq(const Point &p);

    void setPerpendicular(const Point &p);
    void setPerpendicular(const Point &p1, const Point &p2);
};


#endif //VECTORTILEMAP_POINT_H
