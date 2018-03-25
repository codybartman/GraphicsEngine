#ifndef _TerrainModel
#define _TerrainModel

#include "Matrix.h"
#include <tuple>
#include <d3d11.h>
#include "DirectXTex.h"

class Model;
struct ID3D11DeviceContext;
struct ID3D11Device;

class TerrainModel : public Align16
{

private:
	Model * pModTerrain;
	Matrix WorldMat;


public:
	TerrainModel(ID3D11Device* dev, LPCWSTR heightmapFile);

	Matrix GetWorldMat() { return WorldMat; }

	void Render(ID3D11DeviceContext* context);

};



#endif _TerrainModel