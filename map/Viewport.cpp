//
// Created by HB on 5/20/2015.
//

#include "stdafx.h"
#include "Viewport.h"

#include <math.h>
#include <stddef.h>

#include "../Basic.h"
#include "../core/BoundingBox.h"
#include "../core/MercatorProjection.h"
#include "../core/Tile.h"
#include "../utils/FastMath.h"
#include "../utils/thread/Synchronize.h"


Viewport::Viewport()
    : mPos()
    , mProjMatrix()
    , mProjMatrixUnscaled()
    , mProjMatrixInverse()
    , mRotationMatrix()
    , mViewMatrix()
    , mViewProjMatrix()
    , mUnprojMatrix()
    , mTmpMatrix()
    , mMovePoint()
    , mMapBBox()
{
    mPos.scale = MIN_SCALE;
    mPos.x = 0.5;
    mPos.y = 0.5;
    mPos.bearing = 0;
    mPos.tilt = 0;
    mHeight = 0;
    mWidth = 0;
    mtx = NULL;
}


bool Viewport::getMapPosition(MapPosition &pos) {
    synchronized(mtx) {
        bool changed = (pos.scale != mPos.scale
                           || pos.x != mPos.x
                           || pos.y != mPos.y
                           || pos.bearing != mPos.bearing
                           || pos.tilt != mPos.tilt);

        pos.bearing = mPos.bearing;
        pos.tilt = mPos.tilt;

        pos.x = mPos.x;
        pos.y = mPos.y;
        pos.scale = mPos.scale;
        pos.zoomLevel = log2((int) mPos.scale);

        return changed;
    }
}

void Viewport::getMapExtents(float box[], float add) {
    synchronized(mtx) {
        float t = getDepth(1);
        float t2 = getDepth(-1);

        // top-right
        unproject(1, -1, t, box, 0);
        // top-left
        unproject(-1, -1, t, box, 2);
        // bottom-left
        unproject(-1, 1, t2, box, 4);
        // bottom-right
        unproject(1, 1, t2, box, 6);

        if (add == 0)
            return;

        for (int i = 0; i < 8; i += 2) {
            float x = box[i];
            float y = box[i + 1];
            float len = (float) sqrt(x * x + y * y);
            box[i + 0] += x / len * add;
            box[i + 1] += y / len * add;
        }
    }
}

float Viewport::getDepth(float y) {
    if (y == 0)
        return 0;

    // origin is moved by VIEW_DISTANCE
    double cx = VIEW_DISTANCE;
    // 'height' of the ray
    double ry = y * (mHeight / mWidth) * 0.5f;

    double ua;

    if (y == 0)
        ua = 1;
    else {
        // tilt of the plane (center is kept on x = 0)

        double t = toRadians(mPos.tilt);
        double px = y * sin(t);
        double py = y * cos(t);
        ua = 1 + (px * ry) / (py * cx);
    }

    mv[0] = 0;
    mv[1] = (float) (ry / ua);
    mv[2] = (float) (cx - cx / ua);

    mProjMatrixUnscaled.prj(mv);

    return mv[2];
}

void Viewport::unproject(float x, float y, float z, float coords[], int position) {
    mv[0] = x;
    mv[1] = y;
    mv[2] = z;

    mUnprojMatrix.prj(mv);

    coords[position + 0] = mv[0];
    coords[position + 1] = mv[1];
}

void Viewport::getBBox(int expand, BoundingBox &bbox) {
	synchronized(mtx) {
	        getBBox(mMapBBox, expand);

	        /* scale map-pixel coordinates at current scale to
	         * absolute coordinates and apply mercator projection. */
	        double minLon = MercatorProjection::toLongitude(mMapBBox.xmin);
	        double maxLon = MercatorProjection::toLongitude(mMapBBox.xmax);
	        double minLat = MercatorProjection::toLatitude(mMapBBox.ymax);
	        double maxLat = MercatorProjection::toLatitude(mMapBBox.ymin);

	//        BoundingBox *pBBox = new ;
	        bbox.minLatitudeE6 = (int) (minLat * CONVERSION_FACTOR);
	        bbox.maxLatitudeE6 = (int) (maxLat * CONVERSION_FACTOR);
	        bbox.minLongitudeE6 = (int) (minLon * CONVERSION_FACTOR);
	        bbox.maxLongitudeE6 = (int) (maxLon * CONVERSION_FACTOR);

	    }
}

void Viewport::getBBox(BoundingBox &bbox) {
	synchronized(mtx) {
		getBBox(0);
	}
}

BoundingBox* Viewport::getBBox(int expand) {
    synchronized(mtx) {
        getBBox(mMapBBox, expand);

        /* scale map-pixel coordinates at current scale to
         * absolute coordinates and apply mercator projection. */
        double minLon = MercatorProjection::toLongitude(mMapBBox.xmin);
        double maxLon = MercatorProjection::toLongitude(mMapBBox.xmax);
        double minLat = MercatorProjection::toLatitude(mMapBBox.ymax);
        double maxLat = MercatorProjection::toLatitude(mMapBBox.ymin);

        return new BoundingBox(minLat, minLon, maxLat, maxLon);
    }
}

BoundingBox* Viewport::getBBox() {
    synchronized(mtx) {
        return getBBox(0);
    }
}

void Viewport::getBBox(Box &box, int expand) {
    synchronized(mtx) {
        float *coords = mViewCoords;
        getMapExtents(coords, (float)expand);

        box.xmin = coords[0];
        box.xmax = coords[0];
        box.ymin = coords[1];
        box.ymax = coords[1];

        for (int i = 2; i < 8; i += 2) {
//            box.xmin = min(box.xmin, (double) coords[i]);
//            box.xmax = max(box.xmax, (double) coords[i]);
//            box.ymin = min(box.ymin, (double) coords[i + 1]);
//            box.ymax = max(box.ymax, (double) coords[i + 1]);

            box.xmin = (box.xmin < coords[i]) ? box.xmin : coords[i];
            box.xmin = (box.xmax > coords[i]) ? box.xmax : coords[i];
            box.ymin = (box.ymin < coords[i + 1]) ? box.ymin : coords[i];
            box.ymax = (box.ymax < coords[i + 1]) ? box.ymax : coords[i];
        }

        //updatePosition();
        double cs = mPos.scale * Tile::SIZE;
        double cx = mPos.x * cs;
        double cy = mPos.y * cs;

        box.xmin = (cx + box.xmin) / cs;
        box.xmax = (cx + box.xmax) / cs;
        box.ymin = (cy + box.ymin) / cs;
        box.ymax = (cy + box.ymax) / cs;
    }
}

GeoPoint Viewport::fromScreenPoint(float x, float y) {
    synchronized(mtx) {
        fromScreenPoint(x, y, mMovePoint);
        return GeoPoint(
                MercatorProjection::toLatitude(mMovePoint.y),
                MercatorProjection::toLongitude(mMovePoint.x));
    }
}

void Viewport::fromScreenPoint(double x, double y, Point &out) {
    synchronized(mtx) {
        // scale to -1..1
        float mx = (float) (1 - (x / mWidth * 2));
        float my = (float) (1 - (y / mHeight * 2));

        unproject(-mx, my, getDepth(-my), mu, 0);

        double cs = mPos.scale * Tile::SIZE;
        double cx = mPos.x * cs;
        double cy = mPos.y * cs;

        double dx = cx + mu[0];
        double dy = cy + mu[1];

        dx /= cs;
        dy /= cs;

        while (dx > 1)
            dx -= 1;
        while (dx < 0)
            dx += 1;

        if (dy > 1)
            dy = 1;
        else if (dy < 0)
            dy = 0;

        out.x = dx;
        out.y = dy;
    }
}

void Viewport::toScreenPoint(GeoPoint geoPoint, Point &out) {
    synchronized(mtx) {
        MercatorProjection::project(geoPoint, out);
        toScreenPoint(out.x, out.y, out);
    }
}

void Viewport::toScreenPoint(double x, double y, Point &out) {
    synchronized(mtx) {
        double cs = mPos.scale * Tile::SIZE;
        double cx = mPos.x * cs;
        double cy = mPos.y * cs;

        mv[0] = (float) (x * cs - cx);
        mv[1] = (float) (y * cs - cy);

        mv[2] = 0;
        mv[3] = 1;

        mViewProjMatrix.prj(mv);

        out.x = (mv[0] * (mWidth / 2));
        out.y = -(mv[1] * (mHeight / 2));
    }
}

bool Viewport::copy(Viewport *pViewport) {
    synchronized(mtx) {
        mUnprojMatrix.copy(pViewport->mUnprojMatrix);
        mRotationMatrix.copy(pViewport->mRotationMatrix);
        mViewMatrix.copy(pViewport->mViewMatrix);
        mViewProjMatrix.copy(pViewport->mViewProjMatrix);
        return pViewport->getMapPosition(mPos);
    }
}

bool Viewport::copy(Viewport &viewport) {
    synchronized(mtx) {
        mUnprojMatrix.copy(viewport.mUnprojMatrix);
        mRotationMatrix.copy(viewport.mRotationMatrix);
        mViewMatrix.copy(viewport.mViewMatrix);
        mViewProjMatrix.copy(viewport.mViewProjMatrix);
        return viewport.getMapPosition(mPos);
    }
}

void Viewport::initFrom(Viewport *pViewport) {
    synchronized(mtx) {
        mProjMatrix.copy(pViewport->mProjMatrix);
        mProjMatrixUnscaled.copy(pViewport->mProjMatrixUnscaled);
        mProjMatrixInverse.copy(pViewport->mProjMatrixInverse);

        mHeight = pViewport->mHeight;
        mWidth = pViewport->mWidth;
    }
}

void Viewport::initFrom(const Viewport &viewport) {
    synchronized(mtx) {
        mProjMatrix.copy(viewport.mProjMatrix);
        mProjMatrixUnscaled.copy(viewport.mProjMatrixUnscaled);
        mProjMatrixInverse.copy(viewport.mProjMatrixInverse);

        mHeight = viewport.mHeight;
        mWidth = viewport.mWidth;
    }
}


