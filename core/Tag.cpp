//
// Created by HB on 5/19/2015.
//

#include "Tag.h"

std::string KEY_HOUSE_NUMBER = "addr:housenumber";

/** The key of the name OpenStreetMap tag. */
std::string KEY_NAME = "name";

/** The key of the reference OpenStreetMap tag. */
std::string KEY_REF = "ref";

/** The key of the elevation OpenStreetMap tag. */
std::string KEY_ELE = "ele";

std::string KEY_AMENITY = "amenity";
std::string KEY_BUILDING = "building";
std::string KEY_HIGHWAY = "highway";
std::string KEY_LANDUSE = "landuse";
std::string KEY_HEIGHT = "height";
std::string KEY_MIN_HEIGHT = "min_height";

std::string VALUE_YES = "yes";
std::string VALUE_NO = "no";


Tag::Tag()
    : key("")
    , value("")
{
}

Tag::Tag(std::string key, std::string value)
    : key(key)
    , value(value)
{

}

bool Tag::operator==(const Tag &other) {
    if (key != other.key)
        return false;
    if (value != other.value)
        return false;
    return true;
}


Tag &Tag::operator=(const Tag &other) {
    key = other.key;
    value = other.value;
    return *this;
}

Tag Tag::parse(std::string tag) {
    int splitPosition = tag.find("=");
    if (splitPosition < 0)
        return Tag(tag, "");
    return Tag(tag.substr(0, splitPosition), tag.substr(splitPosition + 1));

}
