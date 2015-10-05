//
// Created by HB on 5/22/2015.
//
#include "stdafx.h"
#include "GLViewport.h"
#include "GLMatrix.h"

GLViewport::GLViewport()
    : Viewport()
    , viewproj()
    , proj()
    , view()
    , mvp()
    , pos()
{

}

GLViewport::~GLViewport() {

}

void GLViewport::useScreenCoordinates(bool center, float scale) {
    float ratio = ((float) 1.0 / (scale * mWidth));

    if (center)
        mvp.setScale(ratio, ratio, ratio);
    else
        mvp.setTransScale((-mWidth / 2) * ratio * scale,
                          (-mHeight / 2) * ratio * scale,
                          ratio);

    mvp.multiplyLhs(proj);
}

bool GLViewport::isChanged() const {
    return changed;
}

float GLViewport::getWidth() {
    return mWidth;
}

float GLViewport::getHeight() {
    return mHeight;
}

void GLViewport::setFrom(Viewport *pViewport) {
    changed = copy(pViewport);
	getMapExtents(plane, 0);
}
