//
// Created by HB on 5/19/2015.
//

#ifndef VECTORTILEMAP_MAPPOSITION_H
#define VECTORTILEMAP_MAPPOSITION_H


class GeoPoint;
class BoundingBox;


class MapPosition {

public:
    /** Projected position x 0..1 */
    double x;

    /** Projected position y 0..1 */
    double y;

    /**
     * Absolute scale
     * - use setScale() to modify
     */
    double scale;

    /** Rotation angle */
    float bearing;

    /** Perspective tilt */
    float tilt;

    /**
     * Zoom-level for current scale.
     * - To be removed: FastMath.log2(scale)
     * - use setZoomLevel() to modify
     */
    int zoomLevel;

public:
    MapPosition();
    MapPosition(double lat, double lon, double scale);

public:
    double getX();
    double getY();
    float getBearing();
    float getTilt();

    double getScale();
    int getZoomLevel();

    MapPosition *setX(double x);
    MapPosition *setY(double y);
    MapPosition *setBearing(float bearing);
    MapPosition *setTilt(float tilt);
    MapPosition *setZoomLevel(int zoomLevel);
    MapPosition *setScale(double scale);

    void setPosition(GeoPoint &gp);
    void setPosition(double latitude, double longitude);
    void copy(const MapPosition &other);
    void set(double x, double y, double scale, float bearing, float tilt);

    double getZoomScale();
    GeoPoint getGeoPoint();
    double getLatitude();
    double getLongitude();

    void setByBoundingBox(BoundingBox *pBBox, int viewWidth, int viewHeight);
};


#endif //VECTORTILEMAP_MAPPOSITION_H
