//
// Created by HB on 5/19/2015.
//

#ifndef VECTORTILEMAP_TAG_H
#define VECTORTILEMAP_TAG_H

#include <string>

class Tag {

public:
    /** The key of the house number OpenStreetMap tag. */
    static std::string KEY_HOUSE_NUMBER;

    /** The key of the name OpenStreetMap tag. */
    static std::string KEY_NAME;

    /** The key of the reference OpenStreetMap tag. */
    static std::string KEY_REF;

    /** The key of the elevation OpenStreetMap tag. */
    static std::string KEY_ELE;

    static std::string KEY_AMENITY;
    static std::string KEY_BUILDING;
    static std::string KEY_HIGHWAY;
    static std::string KEY_LANDUSE;
    static std::string KEY_HEIGHT;
    static std::string KEY_MIN_HEIGHT;

    static std::string VALUE_YES;
    static std::string VALUE_NO;

public:
    std::string key;
    std::string value;

public:
    Tag();
    Tag(std::string key, std::string value);


public:
    bool operator == (const Tag& other);
    Tag& operator = (const Tag& other);

    static Tag parse(std::string tag);
};


#endif //VECTORTILEMAP_TAG_H
