#pragma once
#include <string>
#include <map>

class FadeStep;
class ITileDataSource;
class ITileCache;

class Options : public std::map<std::string, std::string> {
public:

};

class OpenResult {
public:
	static OpenResult* SUCCESS;

private:
	char* m_strErrorMessage;
	bool m_bSuccess;


public:
	OpenResult(char* errorMessage = NULL);
	virtual ~OpenResult();

public:
	bool isSuccess();
	char* getErrorMessage();
	char* toString();
};

class TileSource {

public:
	class Builder {
		friend class TileSource;

	protected:
		int m_nZoomMin;
		int m_nZoomMax;
		FadeStep* m_pFadeSteps;

	protected:
		Builder* self() {
			return this;
		}

	public:
		virtual TileSource* build() = 0;
		virtual ~Builder() {};

	public:
		Builder* zoomMin(int zoom);
		Builder* zoomMax(int zoom);
		Builder* fadeSteps(FadeStep *pFadeSteps);

	};

protected:
	int m_nZoomMin;
	int m_nZoomMax;
	FadeStep* m_pFadeSteps;
	Options *m_pOptions;
	ITileCache *m_pTileCache;


protected:
	TileSource(int zoomMin = 0, int zoomMax = 20);		

public:
	TileSource(TileSource::Builder *pBuilder);
	virtual ~TileSource();
	
public:
	virtual ITileDataSource* getDataSource() = 0;
	virtual OpenResult* open() = 0;
	virtual void close() = 0;

public:
	void setCache(ITileCache *pCache);
	int getZoomLevelMax();
	int getZoomLevelMin();
	FadeStep* getFadeSteps();
	TileSource* setOption(std::string key, std::string value);
	std::string getOption(std::string key);
};