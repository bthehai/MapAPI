#include "stdafx.h"
#include "TileSource.h"

OpenResult* OpenResult::SUCCESS = new OpenResult();

OpenResult::OpenResult( char* errorMessage /*= NULL*/ )
{
	if (errorMessage == NULL) {
		m_bSuccess = true;
		m_strErrorMessage = "";
	} else {
		m_bSuccess = false;
		m_strErrorMessage = errorMessage;
	}
}

OpenResult::~OpenResult()
{
	m_strErrorMessage = NULL;
}

bool OpenResult::isSuccess()
{
	return m_bSuccess;
}

char* OpenResult::toString()
{
	char temp[1024];
	sprintf_s(temp, "FileOpenResult [success= %d, errorMessage=%s]", m_bSuccess, m_strErrorMessage);
	return temp;
}

char* OpenResult::getErrorMessage()
{
	return m_strErrorMessage;
}

TileSource::Builder* TileSource::Builder::zoomMin( int zoom )
{
	m_nZoomMin = zoom;
	return self();
}

TileSource::Builder* TileSource::Builder::zoomMax( int zoom )
{
	m_nZoomMax = zoom;
	return self();
}

TileSource::Builder* TileSource::Builder::fadeSteps( FadeStep *pFadeSteps )
{
	m_pFadeSteps = pFadeSteps;
	return self();
}

TileSource::TileSource( int zoomMin /*= 0*/, int zoomMax /*= 20*/ )
	: m_nZoomMin(m_nZoomMin)
	, m_nZoomMax(zoomMax) 
	, m_pFadeSteps(NULL)
{
	m_pTileCache = NULL;
	m_pOptions = new Options();
}

TileSource::TileSource( Builder *pBuilder )
{
	m_nZoomMin = pBuilder->m_nZoomMin;
	m_nZoomMax = pBuilder->m_nZoomMax;	
	m_pFadeSteps = pBuilder->m_pFadeSteps;

	m_pTileCache = NULL;
	m_pOptions = new Options();
}

TileSource::~TileSource()
{
	if (m_pOptions != NULL) {
		delete m_pOptions;
		m_pOptions = NULL;
	}
}

void TileSource::setCache(ITileCache *pCache) {
	m_pTileCache = pCache;
}

int TileSource::getZoomLevelMax() {
	return m_nZoomMax;
}

int TileSource::getZoomLevelMin() {
	return m_nZoomMin;
}

FadeStep* TileSource::getFadeSteps() {
	return m_pFadeSteps;
}

TileSource* TileSource::setOption(std::string key, std::string value) {
	m_pOptions->at(key) = value;
	return this;
}

std::string TileSource::getOption(std::string key) {
	return m_pOptions->at(key);
}