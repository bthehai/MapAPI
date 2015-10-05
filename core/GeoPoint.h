//
// Created by HB on 5/19/2015.
//

#ifndef VECTORTILEMAP_GEOPOINT_H
#define VECTORTILEMAP_GEOPOINT_H


#include "../Basic.h"

class Point;

class GeoPoint : public ICompare<GeoPoint> {

public:
    const int latitudeE6;
    const int longitudeE6;

public:
    GeoPoint(double lat, double lon);
    GeoPoint(int latE6, int lonE6);

private:
    int lat2LatE6(double lat);
    int lon2LonE6(double lon);

public:
    virtual int compareTo(GeoPoint geoPoint);

    bool operator == (const GeoPoint &gp);

public:
    void project(Point &out);

    double getLatitude();
    double getLongitude();

    double distanceTo(const GeoPoint &gp);

    static double distance(double lat1, double lon1, double lat2, double lon2);
};


#endif //VECTORTILEMAP_GEOPOINT_H
