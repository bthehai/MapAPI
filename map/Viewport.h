//
// Created by HB on 5/20/2015.
//

#ifndef VECTORTILEMAP_VIEWPORT_H
#define VECTORTILEMAP_VIEWPORT_H

#include "../core/Box.h"
#include "../core/MapPosition.h"
#include "../core/Point.h"
#include "../renderer/GLMatrix.h"

#define MAX_ZOOMLEVEL 19
#define MIN_ZOOMLEVEL 2

#define MAX_SCALE (1 << MAX_ZOOMLEVEL)
#define MIN_SCALE (1 << MIN_ZOOMLEVEL)

#define MAX_TILT 65

#define VIEW_DISTANCE 3.0
#define VIEW_NEAR 1
#define VIEW_FAR 8
#define VIEW_SCALE (VIEW_NEAR / VIEW_DISTANCE) * 0.5

class Viewport {

protected:
    MapPosition mPos;
    GLMatrix mProjMatrix;
    GLMatrix mProjMatrixUnscaled;
    GLMatrix mProjMatrixInverse;
    GLMatrix mRotationMatrix;
    GLMatrix mViewMatrix;
    GLMatrix mViewProjMatrix;
    GLMatrix mUnprojMatrix;
    GLMatrix mTmpMatrix;

    Point mMovePoint;
    float mv[4];
    float mu[4];
    float mViewCoords[8];

    Box mMapBBox;

    float mHeight;
    float mWidth;

//    std::mutex mtx;
    void* mtx;

protected:
    Viewport();

public:
    bool getMapPosition(MapPosition &pos);
    void getMapExtents(float box[], float add);
    float getDepth(float y);
    void unproject(float x, float y, float z, float coords[], int position);

    void getBBox(int expand, BoundingBox &bbox);
    void getBBox(BoundingBox &bbox);

    BoundingBox* getBBox(int expand);
    BoundingBox* getBBox();

    void getBBox(Box &box, int expand);

    GeoPoint fromScreenPoint(float x, float y);

    void fromScreenPoint(double x, double y, Point &out);

    void toScreenPoint(GeoPoint geoPoint, Point &out);

    void toScreenPoint(double x, double y, Point &out);

    bool copy(Viewport *pViewport);
    bool copy(Viewport &viewport);

    void initFrom(Viewport *pViewport);
    void initFrom(const Viewport &viewport);
};


#endif //VECTORTILEMAP_VIEWPORT_H
