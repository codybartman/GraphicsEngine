// ModelTools
// Support tools to compute bounding volumes on models
// Ported/adapted from Keenan's FBX Converter code
// Andre Berthiaume, June 2016

#ifndef _ModelTools
#define _ModelTools

#include "Vect.h"

struct StandardVertex;
struct TriangleByIndex;

class ModelTools
{
	struct MyVect
	{
		float x;
		float y;
		float z;
		float u;
		float v;
	};

public:
	static void CreatePlane(int vslice, int hslice, StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri, float size);
	static void CreateUnitBox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
	static void CreateUnitBoxRepTexture(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri, float size);
	static void CreateUnitBoxSixFaceTexture(StandardVertex *& pVerts, int & nverts, TriangleByIndex *& pTriList, int & ntri, float size);
	static void CreateSkyBox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri, float BoxSize);
	static void CreateBigPyramid(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
	static void CreateUnitPyramid(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
	static void CreateUnitSphere(int vslice, int hslice, StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);

private:
	static void SetPlanarNormals(StandardVertex *&pVerts, int a, int b, int c);
	static Vect GetNormal(StandardVertex &a, StandardVertex &b, StandardVertex &c);
	static ModelTools::MyVect GetPlaneVert(int side, int hi, int totalSides, int totalHeight, float size);
	static StandardVertex GetCircleVert(int side, int hi, int totalSides, int totalHeight);

};



#endif _ModelTools