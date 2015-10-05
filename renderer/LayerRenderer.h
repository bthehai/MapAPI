#pragma once
#include "GLViewport.h"

class LayerRenderer {
public:
	bool isReady;
	bool isInitialized;

protected:
	void setReady(bool ready) {
		isReady = ready;
	}

public:
	virtual bool setup() {
		return true;
	}

	virtual void update(GLViewport *pViewport) = 0;
	virtual void render(GLViewport *pViewport) = 0;

};