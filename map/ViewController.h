//
// Created by HB on 5/21/2015.
//

#ifndef VECTORTILEMAP_VIEWCONTROLLER_H
#define VECTORTILEMAP_VIEWCONTROLLER_H


#include "Viewport.h"

class ViewController : public Viewport {

public:
    bool tiltMap(float move);
    bool setTilt(float tilt);
    bool scaleMap(float scale, float pivotX, float pivotY);

    void moveMap(float mx, float my);
    void rotateMap(double radians, float pivotX, float pivotY);

    void setScreenSize(int width, int height);
    void setRotation(double degree);
    void setMapPosition(MapPosition mapPosition);

    void moveTo(double x, double y);

private:
    Point applyRotation(double mx, double my);

    void updateMatrices();
};


#endif //VECTORTILEMAP_VIEWCONTROLLER_H
