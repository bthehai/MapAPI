#pragma once
#include "RenderStyle.h"

class AreaStyle : public RenderStyle {
	
public:
	AreaStyle(int color);

public:
	virtual RenderStyle* current();
};