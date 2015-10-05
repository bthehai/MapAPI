//
// Created by HB on 5/19/2015.
//

#ifndef VECTORTILEMAP_BOX_H
#define VECTORTILEMAP_BOX_H


class Point;

class Box {
public:
    double xmin;
    double ymin;
    double xmax;
    double ymax;

public:
    Box();
    Box(double xmin, double ymin, double xmax, double ymax);
    Box(const Box& bbox);

public:
    bool contains(double x, double y);
    bool contains(const Point &p);

    bool inside(const Box &box);

    double getWidth();

    double getHeight();

    bool overlap(const Box &box);
};


#endif //VECTORTILEMAP_BOX_H
