//
// Created by HB on 5/23/2015.
//

#include "MapElement.h"
#include "../Basic.h"

MapElement::MapElement() {
	layer = 0;
	pTags = NULL;
}

MapElement::MapElement(int points, int indices) {
	layer = 0;
	pTags = NULL;
}

MapElement::~MapElement() {

}

void MapElement::setLayer(int layer) {
	this->layer = layer;
}

void MapElement::clear() {
	layer = 5;
}
