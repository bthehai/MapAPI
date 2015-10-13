20151001 1700
- TileManager
- TileSet

20151004:
- TileManager: 
	* Add function:
		+ clearJobs
		+ isEmpty
		+ getTileJob
	Complete class TileManager;
		
20151005 1100:
- Complete TileRenderer class.
- Rename class ScanBoxImpl -> ScanBoxTileManager in TileManager class.

20151010:
- Complete:
	MapApi/tiling folder
	+ ITileCache 
	+ ITileDataSink
	+ ITileDataSource
	+ TileSource
	MapApi/layers/tile folder
	+ TileLoader

- Pauseable extends Thread (Thread undefine, don't chosed thread class in unknown library(boost, lib c++...))
- PasuableThread class: The method or operation is not implemented.

20151013
- Complete:
	layers/tile
	+ TileLayer
	+ VectorTileRenderer
Note:
	Layer/tile
	+ TileLayer
	+ TileRenderer (.h, .cpp)
	renderer/
	+ MapRenderer.cpp
	renderer/bucket
	+ Them static class Renderer trong tung class bucket.
	+ Cac chuc nang trong cac class Bucket chua hoan thanh.


