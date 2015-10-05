//
// Created by HB on 5/23/2015.
//

#ifndef VECTORTILEMAP_MAPELEMENT_H
#define VECTORTILEMAP_MAPELEMENT_H


#include "GeometryBuffer.h"

class TagSet;

class MapElement : public GeometryBuffer {

public:
    int layer;
    TagSet *pTags;

public:
    MapElement();
    MapElement(int points, int indices);

    virtual ~MapElement();

public:
    void setLayer(int layer);

public:
    virtual void clear();

};



#endif //VECTORTILEMAP_MAPELEMENT_H
