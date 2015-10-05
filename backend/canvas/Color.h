#pragma once
#include <string>
#include <math.h>
#include "..\..\utils\FastMath.h"

class Color {
public:
	static unsigned int fadePremul(unsigned int color, double alpha);
	static unsigned int fade(unsigned int color, double alpha);
	static unsigned int rainbow(float pos);
	
	static unsigned int get(int r, int g, int b);
	static unsigned int get(double r, double g, double b);	
	static unsigned int get(int a, int r, int g, int b);	
	static unsigned int get(float a, int r, int g, int b);

	static float rToFloat(unsigned int color);
	static float gToFloat(unsigned int color);
	static float bToFloat(unsigned int color);
	static float aToFloat(unsigned int color);

	static int a(unsigned int color);
	static int r(unsigned int color);
	static int g(unsigned int color);
	static int b(unsigned int color);

	static unsigned int parseColorComponents(std::string str);

private:
	static void parseColorException(std::string str) {
		throw std::exception("Unknown color");
	}


public:
	static unsigned int BLACK;
	static unsigned int DKGRAY;
	static unsigned int GRAY;
	static unsigned int LTGRAY;
	static unsigned int WHITE;
	static unsigned int RED;
	static unsigned int GREEN;
	static unsigned int BLUE;
	static unsigned int YELLOW;
	static unsigned int CYAN;
	static unsigned int MAGENTA;
	static unsigned int TRANSPARENT;

private:
	static unsigned int OPAQUE;

public:
	static unsigned int parseColor(std::string colorString);
	static unsigned int parseColor(std::string colorString, int fallBackColor);
	static bool isOpaque(int color);
};