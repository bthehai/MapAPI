//
// Created by HB on 5/19/2015.
//

#ifndef VECTORTILEMAP_BASIC_H
#define VECTORTILEMAP_BASIC_H

#include <stddef.h>


#ifndef PI
#define PI 3.14159265358979323846
#endif

#define DEG2RAD (PI / 180.0)
#define RAD2DEG (180.0 / PI)
#define RADIUS_EARTH_METERS 6378137

#define toRadians(value) value * DEG2RAD
#define toDegrees(value) value * RAD2DEG

#define CONVERSION_FACTOR 1000000

/**
 * The circumference of the earth at the equator in meters.
 */
#define EARTH_CIRCUMFERENCE 40075016.686

/**
 * Maximum possible latitude coordinate of the map.
 */
#define LATITUDE_MAX 85.05112877980659

/**
 * Minimum possible latitude coordinate of the map.
 */
#define LATITUDE_MIN -LATITUDE_MAX

/**
 * Maximum possible longitude coordinate of the map.
 */
#define LONGITUDE_MAX 180.0

/**
 * Minimum possible longitude coordinate of the map.
 */
#define LONGITUDE_MIN -LONGITUDE_MAX

#ifndef min
#define min(a, b) (a < b) ? a : b
#endif

#ifndef max
#define max(a, b) (a > b) ? a : b
#endif

#ifndef HB_BYTE
#define HB_BYTE unsigned char
#endif
typedef HB_BYTE byte;

template<class TYPE>
class ICompare {
public:
    virtual int compareTo(TYPE other) = 0;
};

template<class T>
class Comparator {
public:
    virtual int compare(T val1, T val2) = 0;
};

template<class E>
class Iterator {
public:
    virtual bool hasNext() = 0;
    virtual E next() = 0;
    virtual void remove() = 0;
};

template<class T>
class Iterable {
public:
    virtual Iterator<T>* iterator() = 0;
};

template<typename T>
void Delete(T &arg) {
}

template<typename T>
void Delete(T *&arg) {
    if (arg != NULL) {
        delete[]arg;
        arg = NULL;
    }
}

#endif //VECTORTILEMAP_BASIC_H
