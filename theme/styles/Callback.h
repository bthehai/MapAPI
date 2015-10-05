#pragma once

#include "AreaStyle.h"
#include "ExtrusionStyle.h"
#include "CircleStyle.h"
#include "SymbolStyle.h"
#include "LineStyle.h"
#include "TextStyle.h"


class Callback {
public:
	virtual void renderArea(AreaStyle *pArea, int level) = 0;
	virtual void renderExtrusion(ExtrusionStyle *pExtrusion, int level) = 0;
	virtual void renderCircle(CircleStyle *pCircle, int level) = 0;
	virtual void renderSymbol(SymbolStyle *pSymbol) = 0;
	virtual void renderWay(LineStyle *pLine, int level) = 0;
	virtual void renderText(TextStyle *pText) = 0;
};
