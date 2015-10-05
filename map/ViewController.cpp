//
// Created by HB on 5/21/2015.
//

#include "stdafx.h"
#include "ViewController.h"
#include <math.h>
#include "../utils/FastMath.h"
#include "../core/Tile.h"
#include "../utils/thread/Synchronize.h"


void ViewController::setScreenSize(int width, int height) {
    synchronized(mtx) {
        mHeight = height;
        mWidth = width;

        /* setup projection matrix:
         * 0. scale to window coordinates
         * 1. translate to VIEW_DISTANCE
         * 2. apply projection
         * setup inverse projection:
         * 0. invert projection
         * 1. invert translate to VIEW_DISTANCE */

        float ratio = (mHeight / mWidth) * VIEW_SCALE;
        float *tmp = new float[16];

        GLMatrix::frustumM(tmp, 0, -VIEW_SCALE, VIEW_SCALE,
                          ratio, -ratio, VIEW_NEAR, VIEW_FAR);

        mProjMatrix.set(tmp);
        mTmpMatrix.setTranslation(0, 0, -VIEW_DISTANCE);
        mProjMatrix.multiplyRhs(mTmpMatrix);

        /* set inverse projection matrix (without scaling) */
        mProjMatrix.get(tmp);
        GLMatrix::invertM(tmp, 0, tmp, 0);
        mProjMatrixInverse.set(tmp);

        mProjMatrixUnscaled.copy(mProjMatrix);

        /* scale to window coordinates */
        mTmpMatrix.setScale(1 / mWidth, 1 / mWidth, 1 / mWidth);
        mProjMatrix.multiplyRhs(mTmpMatrix);

        updateMatrices();
    }
}

void ViewController::moveMap(float mx, float my) {
    synchronized(mtx) {
        Point p = applyRotation(mx, my);
        double tileScale = mPos.scale * Tile::SIZE;
        moveTo(mPos.x - p.x / tileScale, mPos.y - p.y / tileScale);
    }
}

void ViewController::moveTo(double x, double y) {
    mPos.x = x;
    mPos.y = y;

    /* clamp latitude */
    mPos.y = clamp(mPos.y, 0.0, 1.0);

    /* wrap longitude */
    while (mPos.x > 1)
        mPos.x -= 1;
    while (mPos.x < 0)
        mPos.x += 1;
}

Point ViewController::applyRotation(double mx, double my) {
    if (mPos.bearing == 0) {
        mMovePoint.x = mx;
        mMovePoint.y = my;
    } else {
        double rad = toRadians(mPos.bearing);
        double rcos = cos(rad);
        double rsin = sin(rad);
        mMovePoint.x = mx * rcos + my * rsin;
        mMovePoint.y = mx * -rsin + my * rcos;
    }
    return mMovePoint;
}

bool ViewController::scaleMap(float scale, float pivotX, float pivotY) {
    synchronized(mtx) {
        // just sanitize input
        //scale = FastMath.clamp(scale, 0.5f, 2);
        if (scale < 0.000001)
            return false;

        double newScale = mPos.scale * scale;

        newScale = clamp(newScale, (double) MIN_SCALE, (double) MAX_SCALE);

        if (newScale == mPos.scale)
            return false;

        scale = (float) (newScale / mPos.scale);

        mPos.scale = newScale;

        if (pivotX != 0 || pivotY != 0)
            moveMap(pivotX * (1.0f - scale),
                    pivotY * (1.0f - scale));

        return true;
    }
}

void ViewController::rotateMap(double radians, float pivotX, float pivotY) {
    synchronized(mtx) {
        double rsin = sin(radians);
        double rcos = cos(radians);

        float x = (float) (pivotX - pivotX * rcos + pivotY * rsin);
        float y = (float) (pivotY - pivotX * rsin - pivotY * rcos);

        moveMap(x, y);

        setRotation(mPos.bearing + toDegrees(radians));
    }
}

void ViewController::setRotation(double degree) {
    synchronized(mtx) {
        while (degree > 180)
            degree -= 360;
        while (degree < -180)
            degree += 360;

        mPos.bearing = (float) degree;
        updateMatrices();
    }
}

bool ViewController::tiltMap(float move) {
    synchronized(mtx) {
        return setTilt(mPos.tilt + move);
    }
}

bool ViewController::setTilt(float tilt) {
    synchronized(mtx) {
        tilt = clamp(tilt, (float) 0, (float) MAX_TILT);
        if (tilt == mPos.tilt)
            return false;
        mPos.tilt = tilt;
        updateMatrices();
        return true;
    }
}

void ViewController::setMapPosition(MapPosition mapPosition) {
    synchronized(mtx) {
        mPos.scale = clamp(mapPosition.scale, (double) MIN_SCALE, (double) MAX_SCALE);
        mPos.x = mapPosition.x;
        mPos.y = mapPosition.y;
        mPos.tilt = mapPosition.tilt;
        mPos.bearing = mapPosition.bearing;
        updateMatrices();
    }
}

void ViewController::updateMatrices() {
    /* - view matrix:
     * 0. apply rotate
     * 1. apply tilt */

    mRotationMatrix.setRotation(mPos.bearing, 0, 0, 1);
    mTmpMatrix.setRotation(mPos.tilt, 1, 0, 0);

    /* apply first rotation, then tilt */
    mRotationMatrix.multiplyLhs(mTmpMatrix);

    mViewMatrix.copy(mRotationMatrix);

    mViewProjMatrix.multiplyMM(mProjMatrix, mViewMatrix);

    /* inverse projection matrix: */
    /* invert scale */
    mUnprojMatrix.setScale(mWidth, mWidth, 1);

    /* invert rotation and tilt */
    mTmpMatrix.transposeM(mRotationMatrix);

    /* (AB)^-1 = B^-1*A^-1, invert scale, tilt and rotation */
    mTmpMatrix.multiplyLhs(mUnprojMatrix);

    /* (AB)^-1 = B^-1*A^-1, invert projection */
    mUnprojMatrix.multiplyMM(mTmpMatrix, mProjMatrixInverse);
}