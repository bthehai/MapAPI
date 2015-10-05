//
// Created by HB on 5/19/2015.
//

#ifndef VECTORTILEMAP_MERCATORPROJECTION_H
#define VECTORTILEMAP_MERCATORPROJECTION_H


#include "MapPosition.h"
#include "Point.h"
#include "GeoPoint.h"

class MercatorProjection {

private:
    MercatorProjection() {}

public:
    static double groundResolution(double latitude, double scale);
    static float groundResolution(const MapPosition& pos);

    static double latitudeToY(double latitude);
    static double toLatitude(double y);

    static double longitudeToX(double longitude);
    static double toLongitude(double x);

    static Point project(const GeoPoint &p, Point &reuse);
    static void project(const GeoPoint &p, double out[], int pos);
    static void project(double latitude, double longitude, double out[], int pos);
    static double limitLatitude(double latitude);
    static double limitLongitude(double longitude);
    static double wrapLongitude(double longitude);
};


#endif //VECTORTILEMAP_MERCATORPROJECTION_H
