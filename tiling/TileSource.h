#pragma once

class TileSource {

public:
	class Builder {
	protected:
		int zoomMin;
		int zoomMax;

	};

protected:
	int m_nZoomMin;
	int m_nZoomMax;

protected:
	TileSource() {}
	TileSource(int zoomMin, int zoomMax) : m_nZoomMin(m_nZoomMin), m_nZoomMax(zoomMax) {}

public:
	TileSource(Builder builder) {}


};