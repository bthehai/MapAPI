//
// Created by HB on 5/23/2015.
//

#ifndef VECTORTILEMAP_GEOMETRYBUFFER_H
#define VECTORTILEMAP_GEOMETRYBUFFER_H


#include "PointF.h"

class GeometryBuffer {

private:
    enum {
        GROW_INDICES = 64,
        GROW_POINTS = 512,
    };

public:
    enum GeometryType {
        NONE = 0,
        POINT = 1,
        LINE = 2,
        POLY = 3,
        TRIS = 4,
    };

    float *points;
    int pointSize;

    int *index;
    int indexSize;

    int indexPos;
    int pointPos;

    GeometryType type;

private:
    PointF mTmpPoint;
    int pointLimit;


};


#endif //VECTORTILEMAP_GEOMETRYBUFFER_H
