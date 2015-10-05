#pragma once

class Edge {
public:
	float x0, y0, x1, y1, dx, dy;

public:
	void set(float x0, float y0, float x1, float y1) {
		if (y0 <= y1) {
			this->x0 = x0;
			this->y0 = y0;
			this->x1 = x1;
			this->y1 = y1;
		} else {
			this->x0 = x1;
			this->y0 = y1;
			this->x1 = x0;
			this->y1 = y0;
		}
		this->dx = this->x1 - this->x0;
		this->dy = this->y1 - this->y0;
	}
};

class ScanBox {
private:
	float m_Box[8];

	Edge ab;
	Edge bc;
	Edge ca;

	int xmin, xmax;

protected:
	int m_nZoom;

protected:
	virtual void setVisible(int y, int x1, int x2) = 0;

private:
	float* transScale(double x, double y, double scale, int zoom, float box[]);
	void scanTriangle();
	void scanSpans(Edge &e0, Edge &e1);

public:
	void scan(double x, double y, double scale, int zoom, float box[]);
};