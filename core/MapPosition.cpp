//
// Created by HB on 5/19/2015.
//

#include "MapPosition.h"

#include <algorithm>
#include <math.h>

#include "../utils/FastMath.h"
#include "BoundingBox.h"
#include "MercatorProjection.h"
#include "Tile.h"

MapPosition::MapPosition() {
    this->scale = 1;
    this->x = 0.5;
    this->y = 0.5;
    this->zoomLevel = 1;
    this->bearing = 0;
    this->tilt = 0;
}

MapPosition::MapPosition(double lat, double lon, double scale) {

	this->scale = 1;
	this->x = 0.5;
	this->y = 0.5;
	this->zoomLevel = 1;
	this->bearing = 0;
	this->tilt = 0;

    this->setPosition(lat, lon);
    this->setScale(scale);
}

double MapPosition::getX() {
    return x;
}

MapPosition* MapPosition::setX(double x) {
    this->x = x;
    return this;
}

double MapPosition::getY() {
    return y;
}

MapPosition* MapPosition::setY(double y) {
    this->y = y;
    return this;
}

float MapPosition::getBearing() {
    return bearing;
}

MapPosition* MapPosition::setBearing(float bearing) {
    this->bearing = bearing;
    return this;
}

float MapPosition::getTilt() {
    return tilt;
}

MapPosition* MapPosition::setTilt(float tilt) {
    this->tilt = tilt;
    return this;
}

double MapPosition::getScale() {
    return scale;
}

int MapPosition::getZoomLevel() {
    return zoomLevel;
}

MapPosition* MapPosition::setZoomLevel(int zoomLevel) {
    this->zoomLevel = zoomLevel;
    this->scale = 1 << zoomLevel;
    return this;
}

MapPosition* MapPosition::setScale(double scale) {
    this->zoomLevel = log2((int) scale);
    this->scale = scale;
    return this;
}

void MapPosition::setPosition(GeoPoint &gp) {
    double lat = gp.getLatitude();
    double lon = gp.getLongitude();
    this->setPosition(lat, lon);
}

void MapPosition::setPosition(double latitude, double longitude) {
    latitude = MercatorProjection::limitLatitude(latitude);
    longitude = MercatorProjection::limitLongitude(longitude);
    this->x = MercatorProjection::longitudeToX(longitude);
    this->y = MercatorProjection::latitudeToY(latitude);
}

void MapPosition::copy(const MapPosition &other) {
    this->x = other.x;
    this->y = other.y;

    this->bearing = other.bearing;
    this->scale = other.scale;
    this->tilt = other.tilt;
    this->zoomLevel = other.zoomLevel;
}

void MapPosition::set(double x, double y, double scale, float bearing, float tilt) {
    this->x = x;
    this->y = y;
    this->scale = scale;

    while (bearing > 180)
        bearing -= 360;
    while (bearing < -180)
        bearing += 360;
    this->bearing = bearing;

    this->tilt = tilt;
    this->zoomLevel = log2((int) scale);
}

/**
 * @return scale relative to zoom-level.
 */
double MapPosition::getZoomScale() {
    return scale / (1 << zoomLevel);
}

GeoPoint MapPosition::getGeoPoint() {
    return GeoPoint(MercatorProjection::toLatitude(y),
                    MercatorProjection::toLongitude(x));
}

double MapPosition::getLatitude() {
    return MercatorProjection::toLatitude(y);
}

double MapPosition::getLongitude() {
    return MercatorProjection::toLongitude(x);
}

void MapPosition::setByBoundingBox(BoundingBox *pBBox, int viewWidth, int viewHeight) {
    double minx = MercatorProjection::longitudeToX(pBBox->getMinLongitude());
    double miny = MercatorProjection::latitudeToY(pBBox->getMaxLatitude());

    double dx = abs(MercatorProjection::longitudeToX(pBBox->getMaxLongitude()) - minx);
    double dy = abs(MercatorProjection::latitudeToY(pBBox->getMinLatitude()) - miny);
    double zx = viewWidth / (dx * Tile::SIZE);
    double zy = viewHeight / (dy * Tile::SIZE);

    scale = min(zx, zy);
    x = minx + dx / 2;
    y = miny + dy / 2;
    bearing = 0;
    tilt = 0;
}
