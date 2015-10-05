//
// Created by HB on 5/19/2015.
//

#ifndef VECTORTILEMAP_FASTMATH_H
#define VECTORTILEMAP_FASTMATH_H

#include <math.h>

#include "../Basic.h"

#if _MSC_VER < 1800 
inline int round(float a) {
    return (int) floor(a + 0.5);
}
#endif

inline long fround(double a) {
    return (long) floor(a + 0.5);
}

inline int log2(int x) {

    int r = 0; // result of log2(v) will go here

    if ((x & 0xFFFF0000) != 0) {
        x >>= 16;
        r |= 16;
    }
    if ((x & 0xFF00) != 0) {
        x >>= 8;
        r |= 8;
    }
    if ((x & 0xF0) != 0) {
        x >>= 4;
        r |= 4;
    }
    if ((x & 0xC) != 0) {
        x >>= 2;
        r |= 2;
    }
    if ((x & 0x2) != 0) {
        r |= 1;
    }
    return r;
}

inline float pow(int x) {
    if (x == 0)
        return 1;

    return (x > 0 ? (1 << x) : (1.0f / (1 << -x)));
}

inline int clamp(int value, int min, int max) {
    return (value < min ? min : (value > max ? max : value));
}

inline float clamp(float value, float min, float max) {
    return (value < min ? min : (value > max ? max : value));
}

inline double clamp(double value, double min, double max) {
    return (value < min ? min : (value > max ? max : value));
}

inline float clampN(float value) {
    return (value < 0.0f ? 0.0f : (value > 1.0f ? 1.0f : value));
}

inline byte clampToByte(int value) {
    return (byte) (value < 0 ? 0 : (value > 255 ? 255 : value));
}

inline float absMax(float value1, float value2) {
    float a1 = value1 < 0 ? -value1 : value1;
    float a2 = value2 < 0 ? -value2 : value2;
    return a2 < a1 ? a1 : a2;
}

/** test if any absolute value is greater than 'cmp' */
inline bool absMaxCmp(float value1, float value2, float cmp) {
    return value1 < -cmp || value1 > cmp || value2 < -cmp || value2 > cmp;
}

/** test if any absolute value is greater than 'cmp' */
inline bool absMaxCmp(int value1, int value2, int cmp) {
    return value1 < -cmp || value1 > cmp || value2 < -cmp || value2 > cmp;
}

inline bool withinSquaredDist(int dx, int dy, int distance) {
    return dx * dx + dy * dy < distance;
}

inline bool withinSquaredDist(float dx, float dy, float distance) {
    return dx * dx + dy * dy < distance;
}

#endif //VECTORTILEMAP_FASTMATH_H
