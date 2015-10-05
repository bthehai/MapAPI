//
// Created by HB on 5/19/2015.
//

#include <math.h>
#include "GeoPoint.h"
#include "../utils/FastMath.h"
#include "MercatorProjection.h"

GeoPoint::GeoPoint(double lat, double lon)
    : latitudeE6(lat2LatE6(lat))
    , longitudeE6(lon2LonE6(lon))
{
}

GeoPoint::GeoPoint(int latE6, int lonE6)
	: latitudeE6(latE6 / CONVERSION_FACTOR)
	, longitudeE6(lonE6 / CONVERSION_FACTOR)
{

}

int GeoPoint::lat2LatE6(double lat) {
    return (int) (clamp(lat, LATITUDE_MIN, LATITUDE_MAX) * CONVERSION_FACTOR);
}

int GeoPoint::lon2LonE6(double lon) {
    return (int) (clamp(lon, LONGITUDE_MIN, LONGITUDE_MAX) * CONVERSION_FACTOR);
}

int GeoPoint::compareTo(GeoPoint geoPoint) {
    if (longitudeE6 > geoPoint.longitudeE6) {
        return 1;
    } else if (longitudeE6 < geoPoint.longitudeE6) {
        return -1;
    } else if (latitudeE6 > geoPoint.latitudeE6) {
        return 1;
    } else if (latitudeE6 < geoPoint.latitudeE6) {
        return -1;
    }
    return 0;
}

bool GeoPoint::operator==(const GeoPoint &gp) {
    if (latitudeE6 != gp.latitudeE6)
        return false;
    if (longitudeE6 != gp.longitudeE6)
        return false;
    return true;
}

void GeoPoint::project(Point &out) {
    out.x = MercatorProjection::longitudeToX(longitudeE6 / CONVERSION_FACTOR);
    out.y = MercatorProjection::latitudeToY(latitudeE6 / CONVERSION_FACTOR);
}

double GeoPoint::getLatitude() {
    return latitudeE6 / CONVERSION_FACTOR;
}

double GeoPoint::getLongitude() {
    return longitudeE6 / CONVERSION_FACTOR;
}

double GeoPoint::distanceTo(const GeoPoint &gp) {
    return distance(latitudeE6 / 1E6,
                    longitudeE6 / 1E6,
                    gp.latitudeE6 / 1E6,
                    gp.longitudeE6 / 1E6);
}

double GeoPoint::distance(double lat1, double lon1, double lat2, double lon2) {
    double a1 = DEG2RAD * lat1;
    double a2 = DEG2RAD * lon1;
    double b1 = DEG2RAD * lat2;
    double b2 = DEG2RAD * lon2;

    double cosa1 = cos(a1);
    double cosb1 = cos(b1);

    double t1 = cosa1 * cos(a2) * cosb1 * cos(b2);
    double t2 = cosa1 * sin(a2) * cosb1 * sin(b2);

    double t3 = sin(a1) * sin(b1);

    double tt = acos(t1 + t2 + t3);

    return (RADIUS_EARTH_METERS * tt);
}
