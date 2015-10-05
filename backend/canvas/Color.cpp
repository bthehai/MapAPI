#include "stdafx.h"
#include "Color.h"
#include <iostream>

unsigned int Color::OPAQUE = 0xff000000;

unsigned int Color::BLACK	= 0xFF000000;
unsigned int Color::DKGRAY	= 0xFF444444;
unsigned int Color::GRAY	= 0xFF888888;
unsigned int Color::LTGRAY	= 0xFFCCCCCC;
unsigned int Color::WHITE	= 0xFFFFFFFF;
unsigned int Color::RED		= 0xFFFF0000;
unsigned int Color::GREEN	= 0xFF00FF00;
unsigned int Color::BLUE	= 0xFF0000FF;
unsigned int Color::YELLOW	= 0xFFFFFF00;
unsigned int Color::CYAN	= 0xFF00FFFF;
unsigned int Color::MAGENTA = 0xFFFF00FF;
unsigned int Color::TRANSPARENT = 0;

unsigned int Color::fadePremul(unsigned int color, double alpha) {
	alpha = clamp(alpha, 0.0, 1.0);

	alpha *= (color >> 24) & 0xff;
	int c = (((int) alpha) & 0xff) << 24;

	alpha /= 255;

	c |= ((int) (alpha * ((color >> 16) & 0xff))) << 16;
	c |= ((int) (alpha * ((color >> 8) & 0xff))) << 8;
	c |= ((int) (alpha * (color & 0xff)));

	return c;
}

unsigned int Color::fade(unsigned int color, double alpha) {
	alpha = clamp(alpha, 0.0, 1.0);

	alpha *= (color >> 24) & 0xff;
	int c = (((int) alpha) & 0xff) << 24;

	return c | (color & 0x00ffffff);
}

unsigned int Color::rainbow(float pos) {
	float i = 255 * pos;
	int r = (int) round(sin(0.024 * i + 0) * 127 + 128);
	int g = (int) round(sin(0.024 * i + 2) * 127 + 128);
	int b = (int) round(sin(0.024 * i + 4) * 127 + 128);
	return 0xff000000 | (r << 16) | (g << 8) | b;
}

unsigned int Color::get(int r, int g, int b) {
	return 0xff << 24 | r << 16 | g << 8 | b;
}

unsigned int Color::get(double r, double g, double b) {
	return 0xff << 24
		| (int) round(r * 255) << 16
		| (int) round(g * 255) << 8
		| (int) round(b * 255);
}

unsigned int Color::get(int a, int r, int g, int b) {
	return a << 24 | r << 16 | g << 8 | b;
}

unsigned int Color::get(float a, int r, int g, int b) {
	return fade(0xff << 24 | r << 16 | g << 8 | b, a);
}

float Color::rToFloat(unsigned int color) {
	return ((color >> 16) & 0xff) / 255.0f;
}

float Color::gToFloat(unsigned int color) {
	return ((color >> 8) & 0xff) / 255.0f;
}

float Color::bToFloat(unsigned int color) {
	return ((color) & 0xff) / 255.0f;
}

float Color::aToFloat(unsigned int color) {
	return ((color >> 24) & 0xff) / 255.0f;
}

int Color::a(unsigned int color) {
	return ((color >> 24) & 0xff);
}

int Color::r(unsigned int color) {
	return ((color >> 16) & 0xff);
}

int Color::g(unsigned int color) {
	return ((color >> 8) & 0xff);
}

int Color::b(unsigned int color) {
	return ((color) & 0xff);
}

unsigned int Color::parseColorComponents(std::string str) {
	int numComponents = 4;
	int cur = 5;

	if (str.find("rgb(") != -1) {
		numComponents = 3;
		cur = 4;
	} else if (!str.find("rgba("))
		parseColorException(str);

	int end = str.length();
	int component = 0;
	int a = 0, r = 0, g = 0, b = 0;

	if (str.at(end - 1) != ')')
		parseColorException(str);

	for (; cur < end; cur++) {
		char c = str.at(cur);
		if (c == ',') {
			component++;
			if (component >= numComponents)
				parseColorException(str);
			continue;
		} else if (c >= '0' && c <= '9') {
			if (component == 0) {
				r *= 10;
				r += c - '0';
			} else if (component == 1) {
				g *= 10;
				g += c - '0';
			} else if (component == 2) {
				b *= 10;
				b += c - '0';
			} else {
				a *= 10;
				a += c - '0';
			}

		} else
			parseColorException(str);
	}
	if (r > 255 || g > 255 || b > 255 || a > 255)
		parseColorException(str);

	if (numComponents == 3)
		return get(r, g, b);		
	return get(a, r, g, b);

}


unsigned int Color::parseColor(std::string colorString) {
	if (colorString.at(0) == '#') {
		// Use a long to avoid rollovers on #ffXXXXXX	
		std::string temp = "0x" + colorString.substr(1);
		unsigned int color = std::stoul(temp, nullptr, 16);
			
		if (colorString.length() == 7) {
			// Set the alpha value
			color |= 0x00000000ff000000;
		} else if (colorString.length() != 9) {
			parseColorException(colorString);
		}
		return color;
	}
	if (colorString.at(0) == 'r') {
		return parseColorComponents(colorString);
	}
	throw std::exception("Unknown color");
}

unsigned int Color::parseColor(std::string colorString, int fallBackColor) {
	if (colorString.at(0) == '#') {
		// Use a long to avoid rollovers on #ffXXXXXX
		unsigned int color = std::stoul("0x" + colorString.substr(1), nullptr, 16);
		//long color = atoi(colorString.substr(1).c_str());
		if (colorString.length() == 7) {
			// Set the alpha value
			color |= 0x00000000ff000000;
		} else if (colorString.length() != 9) {
			return fallBackColor;
		}
		return color;
	}
	return fallBackColor;
}

bool Color::isOpaque(int color) {
	return (color & OPAQUE) == OPAQUE;
}