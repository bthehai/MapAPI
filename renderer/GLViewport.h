//
// Created by HB on 5/22/2015.
//

#ifndef VECTORTILEMAP_GLVIEWPORT_H
#define VECTORTILEMAP_GLVIEWPORT_H

#include "../map/Viewport.h"
#include "../core/MapPosition.h"

class GLMatrix;

class GLViewport : public Viewport {
public:
    GLMatrix viewproj;
    GLMatrix proj;
    GLMatrix view;
    GLMatrix mvp;
    float plane[8];

    MapPosition pos;

protected:
    bool changed;

public:
    GLViewport();
    virtual ~GLViewport();

public:
    void useScreenCoordinates(bool center, float scale);
    bool isChanged() const;

    float getWidth();
    float getHeight();

public:
    void setFrom(Viewport *pViewport);

};


#endif //VECTORTILEMAP_GLVIEWPORT_H
