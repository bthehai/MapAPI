#pragma once
#include <string>

class Paint {
public:
	enum Align {
		CENTER, LEFT, RIGHT,
	};

	enum Cap {
		BUTT, ROUND, SQUARE
	};

	enum Style {
		FILL, STROKE
	};

	enum FontFamily {
		DEFAULT, DEFAULT_BOLD, MONOSPACE, SANS_SERIF, SERIF
	};

	enum FontStyle {
		BOLD, BOLD_ITALIC, ITALIC, NORMAL
	};

	virtual int getColor() = 0;

	virtual void setColor(int color) = 0; 

	virtual void setStrokeCap(Cap cap) = 0;

	virtual void setStrokeWidth(float width) = 0;

	virtual void setStyle(Style style) = 0;

	virtual void setTextAlign(Align align) = 0;

	virtual void setTextSize(float textSize) = 0;

	virtual void setTypeface(FontFamily fontFamily, FontStyle fontStyle) = 0;

	virtual float measureText(std::string text) = 0;

	virtual float getFontHeight() = 0;

	virtual float getFontDescent() = 0;

};