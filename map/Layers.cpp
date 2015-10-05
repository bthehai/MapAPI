#include "stdafx.h"
#include "Layers.h"
#include "Map.h"
#include "..\event\GestureListener.h"
#include "UpdateListener.h"
#include "InputListener.h"


Layers::Layers( Map *pMap )
	: m_pMap(pMap)
	, m_LayerList()
{	
	m_bDirtyLayers = false;
	m_pLayerRenderers = NULL;
	m_pLayers = NULL;
	m_nLayerRenderers = 0;
}

Layers::~Layers()
{	
	int size = m_LayerList.getSize();
	Layer *pLayer;
	for (int i = 0; i < size; i++) {
		pLayer = m_LayerList.getAt(i);
		if (pLayer != NULL) {
			pLayer->onDetach();
			delete pLayer;
			pLayer = NULL;
		}
	}
	m_LayerList.empty();

	if (m_pLayerRenderers != NULL) {
		delete []m_pLayerRenderers;
		m_pLayerRenderers = NULL;
	}

	if (m_pLayers != NULL) {
		delete []m_pLayers;
		m_pLayers = NULL;
	}
}

void Layers::setDirtyLayers( bool v )
{
	m_bDirtyLayers = v;
}

Layer* Layers::get( int index )
{
	return m_LayerList.getAt(index);
}

Layer* Layers::remove( int index )
{
	m_bDirtyLayers = true;
	
	Layer *pRemove = m_LayerList.getAt(index);
	m_LayerList.removeAt(index);

	if (UpdateListener *pUpdateLayer = dynamic_cast<UpdateListener*>(pRemove))  {
		m_pMap->pEvents->unbind(pUpdateLayer);
	}

	if (InputListener *pInputLayer = dynamic_cast<InputListener*>(pRemove)) {
		m_pMap->pInput->unbind(pInputLayer);
	}

	return pRemove;
}

int Layers::size()
{
	return m_LayerList.getSize();
}

int Layers::rendererSize()
{
	return m_nLayerRenderers;
}

bool Layers::add( Layer *pLayer )
{
	if (m_LayerList.find(pLayer) != -1) {
		return false;
	}
	return m_LayerList.add(pLayer) != -1;
}

void Layers::add( int idx, Layer *pLayer )
{
	if (m_LayerList.find(pLayer) != -1) {
		return;
	}

	if (UpdateListener* pLayerUpdate = dynamic_cast<UpdateListener*>(pLayer)) {		
		m_pMap->pEvents->bind(pLayerUpdate);
	}
	
	if (InputListener* pLayerInput = dynamic_cast<InputListener*>(pLayer)) {
		m_pMap->pInput->bind(pLayerInput);
	}
	m_LayerList.insert(idx, pLayer);
	m_bDirtyLayers = true;
	
}

Layer* Layers::set( int idx, Layer *pLayer )
{
	if (m_LayerList.find(pLayer) != -1) {
		throw std::exception("Layer added twice");
	}
	m_bDirtyLayers = true;
	Layer *pRemove = m_LayerList.getAt(idx);
	m_LayerList.setAt(pLayer, idx);
	
	if (UpdateListener *pUpdateLayer = dynamic_cast<UpdateListener*>(pRemove))  {
		m_pMap->pEvents->unbind(pUpdateLayer);
	}

	if (InputListener *pInputLayer = dynamic_cast<InputListener*>(pRemove)) {
		m_pMap->pInput->unbind(pInputLayer);
	}

	return pRemove;
}

LayerRenderer** Layers::getLayerRenderer()
{
	if (m_bDirtyLayers) {
		updateLayers();
	}
	return m_pLayerRenderers;
}

void Layers::destroy()
{
	if (m_bDirtyLayers) {
		updateLayers();
	}
	int size = m_LayerList.getSize();
	for (int i = 0; i < size; i++) {
		Layer *pLayer = m_LayerList.getAt(i);
		if (pLayer != NULL) {
			pLayer->onDetach();
			delete pLayer;
			pLayer = NULL;
		}
	}
}

bool Layers::handleGesture( Gesture *pGesture, MotionEvent *pMotionEvent )
{
	if (m_bDirtyLayers) {
		updateLayers();
	}
	int size = m_LayerList.getSize();
	for (int i = 0; i < size; i++) {
		Layer *pLayer = m_LayerList.getAt(i);
		if (GestureListener *pGestureListener = dynamic_cast<GestureListener*>(pLayer)) {
			if (pGestureListener->onGesture(pGesture, pMotionEvent)) {
				return true;
			}			
		}
	}
	return false;
}

void Layers::updateLayers()
{
	int size = m_LayerList.getSize();
	int numRenderLayers = 0;
	if (m_pLayers != NULL) {
		delete[]m_pLayers;
		m_pLayers = NULL;
	}

	if (m_pLayerRenderers != NULL) {
		delete []m_pLayerRenderers;
		m_pLayerRenderers = NULL;
	}

	m_pLayers = new Layer *[size];
	for (int i = 0; i < size; i++) {
		Layer *pLayer = m_LayerList.getAt(i);
		if (!pLayer->isEnabled()) {
			continue;
		}
		if (pLayer->getRenderer() != NULL) {
			numRenderLayers++;
		}
		m_pLayers[size - i - 1] = pLayer;
	}

	m_pLayerRenderers = new LayerRenderer*[numRenderLayers];
	m_nLayerRenderers = 0;
	for (int i = 0; i < size; i++) {
		Layer *pLayer = m_LayerList.getAt(i);
		if (!pLayer->isEnabled()) {
			continue;
		}

		LayerRenderer *pLR = pLayer->getRenderer();
		if (pLR != NULL) {
			m_pLayerRenderers[m_nLayerRenderers++] = pLR;
		}
	}
	m_bDirtyLayers = false;
	
}
