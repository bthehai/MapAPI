//
// Created by HB on 5/19/2015.
//

#ifndef VECTORTILEMAP_BOUNDINGBOX_H
#define VECTORTILEMAP_BOUNDINGBOX_H

class GeoPoint;


class BoundingBox {
public:
    int maxLatitudeE6;
    int maxLongitudeE6;
    int minLatitudeE6;
    int minLongitudeE6;

public:
    BoundingBox(int minLatE6, int minLonE6, int maxLatE6, int maxLonE6);
    BoundingBox(double minLat, double minLon, double maxLat, double maxLon);

public:
    bool contains(const GeoPoint &geoPoint);

public:
    GeoPoint getCenterPoint();
    double getMaxLatitude();
    double getMaxLongitude();
    double getMinLatitude();
    double getMinLongitude();

public:
    bool operator == (const BoundingBox &box);
//    BoundingBox& operator = (const BoundingBox &box);

public:
    static BoundingBox fromGeoPoint(GeoPoint *polyline, int size);
};


#endif //VECTORTILEMAP_BOUNDINGBOX_H
