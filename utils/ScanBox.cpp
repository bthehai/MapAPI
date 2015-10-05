#include "stdafx.h"
#include "ScanBox.h"
#include "..\core\Tile.h"
#include <float.h>
#include <math.h>
#include "..\Basic.h"


float* ScanBox::transScale( double x, double y, double scale, int zoom, float box[] )
{
	scale *= Tile::SIZE;

	//double curScale = Tile.SIZE * scale;
	double div = scale / (1 << zoom);

	x *= scale;
	y *= scale;

	for (int i = 0; i < 8; i += 2) {
		m_Box[i + 0] = (float) ((x + box[i + 0]) / div);
		m_Box[i + 1] = (float) ((y + box[i + 1]) / div);
	}
	return m_Box;
}

void ScanBox::scan( double x, double y, double scale, int zoom, float box[] )
{
	m_nZoom = zoom;
	// this does not modify 'box' parameter
	box = transScale(x, y, scale, zoom, box);

	// clip result to min/max as steep angles
	// cause overshooting in x direction.
	float max = FLT_MIN;
	float min = FLT_MAX;

	for (int i = 0; i < 8; i += 2) {
		float xx = box[i];
		if (xx > max)
			max = xx;
		if (xx < min)
			min = xx;
	}

	max = (float) ceil(max);
	min = (float) floor(min);
	if (min == max)
		max++;

	xmin = (int) min;
	xmax = (int) max;

	// top-left -> top-right
	ab.set(box[0], box[1], box[2], box[3]);
	// top-right ->  bottom-right
	bc.set(box[2], box[3], box[4], box[5]);
	// bottom-right -> bottom-left
	ca.set(box[4], box[5], box[0], box[1]);

	scanTriangle();

	// top-left -> bottom-right
	ab.set(box[0], box[1], box[4], box[5]);
	// bottom-right -> bottom-left
	bc.set(box[4], box[5], box[6], box[7]);
	// bottom-left -> top-left
	ca.set(box[6], box[7], box[0], box[1]);

	scanTriangle();
}

void ScanBox::scanTriangle()
{
	// sort so that ca.dy > bc.dy > ab.dy
	if (ab.dy > bc.dy) {
		Edge t = ab;
		ab = bc;
		bc = t;
	}
	if (ab.dy > ca.dy) {
		Edge t = ab;
		ab = ca;
		ca = t;
	}
	if (bc.dy > ca.dy) {
		Edge t = bc;
		bc = ca;
		ca = t;
	}

	// shouldnt be possible, anyway
	if (ca.dy == 0)
		return;

	if (ab.dy > 0.0)
		scanSpans(ca, ab);

	if (bc.dy > 0.0)
		scanSpans(ca, bc);
}

void ScanBox::scanSpans( Edge &e0, Edge &e1 )
{
	// scan the y-range of the edge with less dy
	int y0 = (int) max(0, floor(e1.y0));
	int y1 = (int) min((1 << m_nZoom), ceil(e1.y1));

	// sort edge by x-coordinate
	if (e0.x0 == e1.x0 && e0.y0 == e1.y0) {
		// bottom-flat
		if (e0.x0 + e1.dy / e0.dy * e0.dx < e1.x1) {
			Edge t = e0;
			e0 = e1;
			e1 = t;
		}
	} else {
		// top-flat
		if (e0.x1 - e1.dy / e0.dy * e0.dx < e1.x0) {
			Edge t = e0;
			e0 = e1;
			e1 = t;
		}
	}

	float m0 = e0.dx / e0.dy;
	float m1 = e1.dx / e1.dy;

	// e0 goes to the right, e1 to the left
	int d0 = e0.dx > 0 ? 1 : 0; // use y + 1 to compute x0
	int d1 = e1.dx < 0 ? 1 : 0; // use y + 1 to compute x1
	float dy;

	for (int y = y0; y < y1; y++) {

		dy = d0 + y - e0.y0;
		if (dy > e0.dy)
			dy = e0.dy;

		int x0 = (int) ceil(e0.x0 + m0 * dy);

		dy = d1 + y - e1.y0;
		if (dy > e1.dy)
			dy = e1.dy;

		int x1 = (int) floor(e1.x0 + m1 * dy);

		if (x1 < xmin)
			x1 = xmin;

		if (x0 > xmax)
			x0 = xmax;

		if (x1 < x0)
			setVisible(y, x1, x0);
	}
}
