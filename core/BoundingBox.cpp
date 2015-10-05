//
// Created by HB on 5/19/2015.
//

#include "BoundingBox.h"

#include <limits.h>
#include <algorithm>

#include "GeoPoint.h"

BoundingBox::BoundingBox(int minLatE6, int minLonE6, int maxLatE6, int maxLonE6)
    : minLatitudeE6(minLatE6)
    , minLongitudeE6(minLonE6)
    , maxLatitudeE6(maxLatE6)
    , maxLongitudeE6(maxLonE6)
{
}

BoundingBox::BoundingBox(double minLat, double minLon, double maxLat, double maxLon)
    : minLatitudeE6((int) (minLat * CONVERSION_FACTOR))
	, minLongitudeE6((int) (minLon * CONVERSION_FACTOR))
	, maxLatitudeE6((int) (maxLat * CONVERSION_FACTOR))
	, maxLongitudeE6((int) (maxLon * CONVERSION_FACTOR))
{
}

bool BoundingBox::contains(const GeoPoint &geoPoint) {
    return geoPoint.latitudeE6 <= maxLatitudeE6
           && geoPoint.latitudeE6 >= minLatitudeE6
           && geoPoint.longitudeE6 <= maxLongitudeE6
           && geoPoint.longitudeE6 >= minLongitudeE6;
}

GeoPoint BoundingBox::getCenterPoint() {
    int latitudeOffset = (maxLatitudeE6 - minLatitudeE6) / 2;
    int longitudeOffset = (maxLongitudeE6 - minLongitudeE6) / 2;
    return GeoPoint(minLatitudeE6 + latitudeOffset,
                    minLongitudeE6 + longitudeOffset);
}

double BoundingBox::getMaxLatitude() {
    return maxLatitudeE6 / CONVERSION_FACTOR;
}

double BoundingBox::getMaxLongitude() {
    return maxLongitudeE6 / CONVERSION_FACTOR;
}

double BoundingBox::getMinLatitude() {
    return minLatitudeE6 / CONVERSION_FACTOR;
}

double BoundingBox::getMinLongitude() {
    return minLongitudeE6 / CONVERSION_FACTOR;
}

bool BoundingBox::operator==(const BoundingBox &other) {
    if (maxLatitudeE6 != other.maxLatitudeE6) {
        return false;
    } else if (maxLongitudeE6 != other.maxLongitudeE6) {
        return false;
    } else if (minLatitudeE6 != other.minLatitudeE6) {
        return false;
    } else if (minLongitudeE6 != other.minLongitudeE6) {
        return false;
    }
    return true;
}

//BoundingBox& BoundingBox::operator = (const BoundingBox &box) {
//	minLatitudeE6 = box.minLatitudeE6;
//	maxLatitudeE6 = box.maxLatitudeE6;
//	minLongitudeE6 = box.minLongitudeE6;
//	maxLongitudeE6 = box.maxLongitudeE6;
//	return *this;
//}

BoundingBox BoundingBox::fromGeoPoint(GeoPoint *polyline, int size) {
    int minLat = INT_MAX;
    int minLon = INT_MAX;
    int maxLat = INT_MIN;
    int maxLon = INT_MIN;
    for (int i = 0; i < size; i++) {
        GeoPoint gp = polyline[i];
        minLat = min(minLat, gp.latitudeE6);
        minLon = min(minLon, gp.longitudeE6);
        maxLat = max(maxLat, gp.latitudeE6);
        maxLon = max(maxLon, gp.longitudeE6);
    }

    return BoundingBox(minLat, minLon, maxLat, maxLon);
}
