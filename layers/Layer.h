#pragma once

class Map;
class LayerRenderer;

class Layer {
public:
	Layer(Map *pMap) 
		: m_pMap(pMap)
		, m_pRenderer(NULL)
		, m_bEnabled(true)
	{
	}

	virtual ~Layer() {};

private:
	bool m_bEnabled;

protected:
	const Map *m_pMap;

	LayerRenderer* m_pRenderer;

public:
	LayerRenderer* getRenderer() {
		return m_pRenderer;
	}

	void setEnabled(bool enabled) {
		m_bEnabled = enabled;
	}

	bool isEnabled() {
		return m_bEnabled;
	}

	const Map* map() {
		return m_pMap;
	}

public:
	virtual void onDetach() {
	}

};