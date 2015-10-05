#pragma once
#include <string>
#include "..\..\backend\canvas\Color.h"

class Callback;

class RenderStyle {
public:
	static class StyleBuilder {
	public:
		std::string style;

		int level;

		int fillColor;

		unsigned int strokeColor;
		float strokeWidth;

	public:
		StyleBuilder* setStyle(std::string style) {
			this->style = style;
			return this;
		}

		StyleBuilder* setLevel(int level) {
			this->level = level;
			return this;
		}

		StyleBuilder* outline(unsigned int color, float width) {
			this->strokeColor = color;
			this->strokeWidth = width;
			return this;
		}

		StyleBuilder* setStrokeColor(unsigned int color) {
			this->strokeColor = color;
			return this;
		}

		StyleBuilder* setStrokeColor(std::string color) {
			this->strokeColor = Color::parseColor(color);
			return this;
		}

		StyleBuilder* setStrokeWidth(float width) {
			this->strokeWidth = width;
			return this;
		}

		StyleBuilder* color(int color) {
			this->fillColor = color;
			return this;
		}

	public:
		virtual RenderStyle* build() = 0;
	};	

public:
	RenderStyle *m_pCurrent;
	RenderStyle *m_pNext;
	bool update_;

public:
	RenderStyle() {
		m_pCurrent = this;
		m_pNext = NULL;
		update_ = false;
	}

	virtual ~RenderStyle() {
	}

public:
	void set(RenderStyle *pNext) {
		update_ = true;
		m_pNext = pNext;
	}

	void unsetOverride() {
		update_ = true;
		m_pNext = NULL;
	}

	void update() {
		if (update_) {
			update_ = false;
			m_pCurrent = m_pNext;
		}
	}

	virtual void dispose() {}
	virtual void renderNode(Callback *pRenderCallback) {}
	virtual void renderWay(Callback *pRenderCallback) {}
	virtual void scaleTextSize(float scaleFactor) {}

	virtual RenderStyle* current() = 0;

};