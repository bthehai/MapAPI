//
// Created by HB on 5/19/2015.
//

#ifndef VECTORTILEMAP_TAGSET_H
#define VECTORTILEMAP_TAGSET_H

#include <string>

using namespace std;

class Tag;

class TagSet {
public:
    Tag *tags;
    int numTags;

private:
    int capacity;

public:
    TagSet();
    TagSet(int size);

    ~TagSet();

public:
    void clear();
    void clearAndNullTags();

    Tag* asArray(int &length);
    Tag& get(std::string key);
    bool containsKey(std::string key);
    std::string getValue(std::string key);

    void add(const Tag &tag);
    void set(Tag *tagArray, int length);

    bool contains(const Tag &tag);
    bool contains(std::string key, std::string value);
};


#endif //VECTORTILEMAP_TAGSET_H
