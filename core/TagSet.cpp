//
// Created by HB on 5/19/2015.
//

#include "TagSet.h"

#include <memory.h>

#include "Tag.h"

TagSet::TagSet()
    : tags(new Tag[10])
    , capacity(10)
    , numTags(0)
{
}

TagSet::TagSet(int size) {
    tags = new Tag[size];
    this->capacity = size;
    numTags = 0;
}

TagSet::~TagSet() {
    if (tags != NULL) {
        delete []tags;
        tags = NULL;
    }
}

void TagSet::clear() {
    numTags = 0;
}

void TagSet::clearAndNullTags() {
    memset(tags, 0, capacity * sizeof(Tag));
    numTags = 0;
}

Tag *TagSet::asArray(int &length) {
    length = numTags;
    Tag *temp = new Tag[numTags];
    memcpy(temp, tags, numTags * sizeof(Tag));
    return temp;
}

Tag& TagSet::get(std::string key) {
    for (int i = 0; i < numTags; i++) {
        if (tags[i].key == key)
            return tags[i];
    }
    return tags[0];
}

bool TagSet::containsKey(std::string key) {
    for (int i = 0; i < numTags; i++) {
        if (tags[i].key == key)
            return true;
    }
    return false;
}

std::string TagSet::getValue(std::string key) {
    for (int i = 0; i < numTags; i++) {
        if (tags[i].key == key)
            return tags[i].value;
    }
    return "";
}

void TagSet::add(const Tag &tag) {
    if (numTags >= capacity) {
        Tag *tmp = tags;

        tags = new Tag[numTags + 4];
        memcpy(tags, tmp, numTags * sizeof(Tag));
        delete []tmp;
        capacity += 4;
    }
    tags[numTags++] = tag;

}

void TagSet::set(Tag *tagArray, int length) {
    int newTags = length;
    if (newTags > numTags) {
        tags = new Tag[length];
    }
    memcpy(tags, tagArray,  newTags * sizeof(Tag));
    numTags = newTags;
}

bool TagSet::contains(const Tag &tag) {
    for (int i = 0; i < numTags; i++) {
        Tag t = tags[i];
        if (t.key == tag.key && t.value == tag.value)
            return true;
    }
    return false;
}

bool TagSet::contains(std::string key, std::string value) {
    for (int i = 0; i < numTags; i++) {
        Tag t = tags[i];
        if (t.key == key && t.value == value)
            return true;
    }
    return false;
}
