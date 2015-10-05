//
// Created by HB on 5/19/2015.
//

#include <math.h>
#include "MercatorProjection.h"
#include "Tile.h"
#include "../utils/FastMath.h"

double MercatorProjection::groundResolution(double latitude, double scale) {
    return cos(latitude * DEG2RAD) * EARTH_CIRCUMFERENCE / (Tile::SIZE * scale);
}

float MercatorProjection::groundResolution(const MapPosition &pos) {
    double lat = MercatorProjection::toLatitude(pos.y);
    return (float) (cos(lat * DEG2RAD) * EARTH_CIRCUMFERENCE / (Tile::SIZE * pos.scale));
}

double MercatorProjection::latitudeToY(double latitude) {
    double sinLatitude = sin(latitude * DEG2RAD);
    return 0.5 - log((1 + sinLatitude) / (1 - sinLatitude)) / (4 * PI);
}

double MercatorProjection::toLatitude(double y) {
    return 90 - 360 * atan(exp((y - 0.5) * (2 * PI))) / PI;
}

double MercatorProjection::longitudeToX(double longitude) {
    return (longitude + 180.0) / 360.0;
}

double MercatorProjection::toLongitude(double x) {
    return 360.0 * (x - 0.5);
}

Point MercatorProjection::project(const GeoPoint &p, Point &reuse) {
    reuse.x = ((p.longitudeE6 / 1E6) + 180.0) / 360.0;

    double sinLatitude = sin((p.latitudeE6 / 1E6) * DEG2RAD);
    reuse.y = 0.5 - log((1.0 + sinLatitude) / (1.0 - sinLatitude)) / (4.0 * PI);

    return reuse;
}

void MercatorProjection::project(const GeoPoint &p, double out[], int pos) {
    out[pos * 2] = ((p.longitudeE6 / 1E6) + 180.0) / 360.0;

    double sinLatitude = sin((p.latitudeE6 / 1E6) * DEG2RAD);
    out[pos * 2 + 1] = 0.5 - log((1.0 + sinLatitude) / (1.0 - sinLatitude)) / (4.0 * PI);
}

void MercatorProjection::project(double latitude, double longitude, double out[], int pos) {
    out[pos * 2] = (longitude + 180.0) / 360.0;

    double sinLatitude = sin(latitude * DEG2RAD);
    out[pos * 2 + 1] = 0.5 - log((1.0 + sinLatitude) / (1.0 - sinLatitude)) / (4.0 * PI);
}

double MercatorProjection::limitLatitude(double latitude) {
    return clamp(latitude, LATITUDE_MIN, LATITUDE_MAX);
}

double MercatorProjection::limitLongitude(double longitude) {
    return clamp(longitude, LONGITUDE_MIN, LONGITUDE_MAX);
}

double MercatorProjection::wrapLongitude(double longitude) {

    if (longitude < -180)
        return clamp(360 + longitude, LONGITUDE_MIN, LONGITUDE_MAX);
    else if (longitude > 180)
        return clamp(longitude - 360, LONGITUDE_MIN, LONGITUDE_MAX);

    return longitude;
}
