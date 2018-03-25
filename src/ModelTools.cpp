#include "ModelTools.h"
//#include "Enum.h"
//#include <math.h>
#include "Matrix.h"
#include <assert.h>
#include "Model.h"
#include "d3dUtil.h"
#include "math.h"

void ModelTools::SetPlanarNormals(StandardVertex *&pVerts, int a, int b, int c)
{
	Vect n;
	n = GetNormal(pVerts[a], pVerts[b], pVerts[c]);

	pVerts[a].SetNormal(n);
	pVerts[b].SetNormal(n);
	pVerts[c].SetNormal(n);

}

Vect ModelTools::GetNormal(StandardVertex &a, StandardVertex &b, StandardVertex &c)
{
	Vect u = a.Pos - b.Pos;
	Vect v = b.Pos - c.Pos;

	return u.cross(v);
}



ModelTools::MyVect ModelTools::GetPlaneVert(int side, int hi, int totalSides, int totalHeight, float size)
{
	float percent = ((float)side / (float)totalSides);
	float heightRatio = ((float)hi / totalHeight);
	float quarterSize = size / 4;

	ModelTools::MyVect returnVect;
	returnVect.x = (percent * size) - quarterSize;
	returnVect.y = 0;
	returnVect.z = (heightRatio * size) - quarterSize;
	returnVect.u = (float)side;
	returnVect.v = (float)hi;
	return returnVect;

}

void ModelTools::CreatePlane(int vslice, int hslice, StandardVertex *& pVerts, int & nverts, TriangleByIndex *& pTriList, int & ntri, float size)
{

	nverts = vslice * hslice;
	pVerts = new StandardVertex[nverts];
	ntri = 2 * ((vslice - 1) * (hslice - 1));
	pTriList = new TriangleByIndex[ntri];
	//int index[30][30];

	for (int hi = 0; hi < hslice; hi++)
	{
		for (int vi = 0; vi < vslice; vi++)
		{
			ModelTools::MyVect vectIn = GetPlaneVert(vi, hi, vslice, hslice, size);
			int index = (hi * hslice) + vi;
			pVerts[index].set(vectIn.x, vectIn.y, vectIn.z, vectIn.u, vectIn.v);
			Vect normal(0.0f, 1.0f, 0.0f);
			pVerts[index].SetNormal(normal);
			//index[hi] [vi] = 
		}
	}

	int maxhi = hslice - 1;
	int maxvi = vslice - 1;
	int middleVert = maxhi * maxvi;

	for (int col = 0; col < maxhi; col++)
	{
		for (int row = 0; row < maxvi; row++)
		{
			int index1 = (maxvi * col) + row;
			int index2 = index1 + middleVert;
			int rowOffset0 = (vslice * col) + row;
			int rowOffset1 = rowOffset0 + hslice;
			pTriList[index1].set(rowOffset0 + 1, rowOffset0, rowOffset1 + 1);
			pTriList[index2].set(rowOffset0, rowOffset1, rowOffset1 + 1);
		}
	}
}


/// Creates the unit box centered at the origin
void ModelTools::CreateUnitBox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri)
{
	nverts = 8;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];

	pVerts[0].set(-0.5f, -0.5f, -0.5f, Colors::Black);
	pVerts[1].set(-0.5f, +0.5f, -0.5f, Colors::Lime);
	pVerts[2].set(+0.5f, +0.5f, -0.5f, Colors::Yellow);
	pVerts[3].set(+0.5f, -0.5f, -0.5f, Colors::Red);
	pVerts[4].set(-0.5f, -0.5f, +0.5f, Colors::Blue);
	pVerts[5].set(-0.5f, +0.5f, +0.5f, Colors::Cyan);
	pVerts[6].set(+0.5f, +0.5f, +0.5f, Colors::White);
	pVerts[7].set(+0.5f, -0.5f, +0.5f, Colors::Magenta);

	// back face
	pTriList[0].set(0, 1, 2);
	pTriList[1].set(0, 2, 3);

	// front face
	pTriList[2].set(4, 6, 5);
	pTriList[3].set(4, 7, 6);

	// left face
	pTriList[4].set(4, 5, 1);
	pTriList[5].set(4, 1, 0);

	// right face
	pTriList[6].set(3, 2, 6);
	pTriList[7].set(3, 6, 7);

	// top face
	pTriList[8].set(1, 5, 6);
	pTriList[9].set(1, 6, 2);

	// bottom face
	pTriList[10].set(4, 0, 3);
	pTriList[11].set(4, 3, 7);
}

void ModelTools::CreateUnitBoxRepTexture(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri, float sizeIn)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(sizeIn, sizeIn, sizeIn, 1, 0, 0, 0, 1);
	pVerts[vind + 1].set(-sizeIn, sizeIn, sizeIn, 0, 0, 0, 0, 1);
	pVerts[vind + 2].set(-sizeIn, -sizeIn, sizeIn, 0, 1, 0, 0, 1);
	pVerts[vind + 3].set(sizeIn, -sizeIn, sizeIn, 1, 1, 0, 0, 1);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(sizeIn, sizeIn, -sizeIn, 0, 0, 0, 0, -1);
	pVerts[vind + 1].set(-sizeIn, sizeIn, -sizeIn, 1, 0, 0, 0, -1);
	pVerts[vind + 2].set(-sizeIn, -sizeIn, -sizeIn, 1, 1, 0, 0, -1);
	pVerts[vind + 3].set(sizeIn, -sizeIn, -sizeIn, 0, 1, 0, 0, -1);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(sizeIn, sizeIn, -sizeIn, 1, 0, 1, 0, 0);
	pVerts[vind + 1].set(sizeIn, sizeIn, sizeIn, 0, 0, 1, 0, 0);
	pVerts[vind + 2].set(sizeIn, -sizeIn, sizeIn, 0, 1, 1, 0, 0);
	pVerts[vind + 3].set(sizeIn, -sizeIn, -sizeIn, 1, 1, 1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-sizeIn, sizeIn, sizeIn, 1, 0, -1, 0, 0);
	pVerts[vind + 1].set(-sizeIn, sizeIn, -sizeIn, 0, 0, -1, 0, 0);
	pVerts[vind + 2].set(-sizeIn, -sizeIn, -sizeIn, 0, 1, -1, 0, 0);
	pVerts[vind + 3].set(-sizeIn, -sizeIn, sizeIn, 1, 1, -1, 0, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(sizeIn, sizeIn, -sizeIn, 1, 0, 0, 1, 0);
	pVerts[vind + 1].set(-sizeIn, sizeIn, -sizeIn, 0, 0, 0, 1, 0);
	pVerts[vind + 2].set(-sizeIn, sizeIn, sizeIn, 0, 1, 0, 1, 0);
	pVerts[vind + 3].set(sizeIn, sizeIn, sizeIn, 1, 1, 0, 1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(sizeIn, -sizeIn, sizeIn, 1, 0, 0, -1, 0);
	pVerts[vind + 1].set(-sizeIn, -sizeIn, sizeIn, 0, 0, 0, -1, 0);
	pVerts[vind + 2].set(-sizeIn, -sizeIn, -sizeIn, 0, 1, 0, -1, 0);
	pVerts[vind + 3].set(sizeIn, -sizeIn, -sizeIn, 1, 1, 0, -1, 0);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateUnitBoxSixFaceTexture(StandardVertex *& pVerts, int & nverts, TriangleByIndex *& pTriList, int & ntri, float sizeIn)
{

	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];
	float one = (1.0f / 8.0f);
	float two = (2.0f / 8.0f);
	float thr = (3.0f / 8.0f);
	float fou = (4.0f / 8.0f);
	float fiv = (5.0f / 8.0f);
	float six = (6.0f / 8.0f);
	float sev = (7.0f / 8.0f);
	float eig = (8.0f / 8.0f);
	Vect forward = Vect(0, 0, 1);
	Vect back = Vect(0, 0, -1);
	Vect left = Vect(1, 0, 0);
	Vect right = Vect(-1, 0, 0);
	Vect down = Vect(0, -1, 0);



	// Setting up faces
	// Forward
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(+sizeIn, +sizeIn, +sizeIn, two, fiv, forward);
	pVerts[vind + 1].set(-sizeIn, +sizeIn, +sizeIn, two, thr, forward);
	pVerts[vind + 2].set(-sizeIn, -sizeIn, +sizeIn, 0, thr, forward);
	pVerts[vind + 3].set(+sizeIn, -sizeIn, +sizeIn, 0, fiv, forward);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(+sizeIn, +sizeIn, -sizeIn, fou, fiv, back);
	pVerts[vind + 1].set(-sizeIn, +sizeIn, -sizeIn, fou, thr, back);
	pVerts[vind + 2].set(-sizeIn, -sizeIn, -sizeIn, six, thr, back);
	pVerts[vind + 3].set(+sizeIn, -sizeIn, -sizeIn, six, fiv, back);
	pTriList[tind].set(vind + 2, vind + 1, vind);
	pTriList[tind + 1].set(vind + 3, vind + 2, vind);

	// Left
	vind += 4;
	tind += 2;
	pVerts[vind].set(sizeIn, +sizeIn, -sizeIn, fou, fiv, left);
	pVerts[vind + 1].set(sizeIn, +sizeIn, +sizeIn, two, fiv, left);
	pVerts[vind + 2].set(sizeIn, -sizeIn, +sizeIn, two, sev, left);
	pVerts[vind + 3].set(sizeIn, -sizeIn, -sizeIn, fou, sev, left);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Right
	vind += 4;
	tind += 2;
	pVerts[vind].set(-sizeIn, +sizeIn, +sizeIn, two, thr, right);
	pVerts[vind + 1].set(-sizeIn, +sizeIn, -sizeIn, fou, thr, right);
	pVerts[vind + 2].set(-sizeIn, -sizeIn, -sizeIn, fou, one, right);
	pVerts[vind + 3].set(-sizeIn, -sizeIn, +sizeIn, two, one, right);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Top
	vind += 4;
	tind += 2;
	pVerts[vind].set(+sizeIn, sizeIn, -sizeIn, two, fiv);
	pVerts[vind + 1].set(-sizeIn, sizeIn, -sizeIn, two, thr);
	pVerts[vind + 2].set(-sizeIn, sizeIn, +sizeIn, fou, thr);
	pVerts[vind + 3].set(+sizeIn, sizeIn, +sizeIn, fou, fiv);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Bottom
	vind += 4;
	tind += 2;
	pVerts[vind].set(+sizeIn, -sizeIn, +sizeIn, eig, fiv, down);
	pVerts[vind + 1].set(-sizeIn, -sizeIn, +sizeIn, eig, thr, down);
	pVerts[vind + 2].set(-sizeIn, -sizeIn, -sizeIn, six, thr, down);
	pVerts[vind + 3].set(+sizeIn, -sizeIn, -sizeIn, six, fiv, down);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);
}

void ModelTools::CreateSkyBox(StandardVertex *& pVerts, int & nverts, TriangleByIndex *& pTriList, int & ntri, float BoxSize)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	pTriList = new TriangleByIndex[ntri];
	float zer = 0.0f;
	float one = (1.0f / 8.0f);
	float two = (2.0f / 8.0f);
	float thr = (3.0f / 8.0f);
	float fou = (4.0f / 8.0f);
	float fiv = (5.0f / 8.0f);
	float six = (6.0f / 8.0f);
	float sev = (7.0f / 8.0f);
	float eig = (8.0f / 8.0f);

	//Set all normals
	for (int i = 0; i < nverts; i++)
	{
		Vect normal(0, 0, 0);
		pVerts[i].SetNormal(normal);
	}


	// Setting up faces
	// Forward 
	int vind = 0;
	int tind = 0;
	pVerts[vind].set(+BoxSize, +BoxSize, +BoxSize, fou, thr);
	pVerts[vind + 1].set(-BoxSize, +BoxSize, +BoxSize, six, thr);
	pVerts[vind + 2].set(-BoxSize, -BoxSize, +BoxSize, six, fiv);
	pVerts[vind + 3].set(+BoxSize, -BoxSize, +BoxSize, fou, fiv);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Back
	vind += 4;
	tind += 2;
	pVerts[vind].set(+BoxSize, +BoxSize, -BoxSize, two, thr);
	pVerts[vind + 1].set(-BoxSize, +BoxSize, -BoxSize, zer, thr);
	pVerts[vind + 2].set(-BoxSize, -BoxSize, -BoxSize, zer, fiv);
	pVerts[vind + 3].set(+BoxSize, -BoxSize, -BoxSize, two, fiv);
	pTriList[tind].set(vind, vind + 1, vind + 2);
	pTriList[tind + 1].set(vind, vind + 2, vind + 3);

	// Left 
	vind += 4;
	tind += 2;
	pVerts[vind].set(BoxSize, +BoxSize, -BoxSize, two, thr);
	pVerts[vind + 1].set(BoxSize, +BoxSize, +BoxSize, fou, thr);
	pVerts[vind + 2].set(BoxSize, -BoxSize, +BoxSize, fou, fiv);
	pVerts[vind + 3].set(BoxSize, -BoxSize, -BoxSize, two, fiv);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);


	// Right //this one
	vind += 4;
	tind += 2;
	pVerts[vind].set(-BoxSize, +BoxSize, +BoxSize, six, thr);
	pVerts[vind + 1].set(-BoxSize, +BoxSize, -BoxSize, eig, thr);
	pVerts[vind + 2].set(-BoxSize, -BoxSize, -BoxSize, eig, fiv);
	pVerts[vind + 3].set(-BoxSize, -BoxSize, +BoxSize, six, fiv);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Top 
	vind += 4;
	tind += 2;
	pVerts[vind].set(+BoxSize, BoxSize, -BoxSize, two, thr);
	pVerts[vind + 1].set(-BoxSize, BoxSize, -BoxSize, two, one);
	pVerts[vind + 2].set(-BoxSize, BoxSize, +BoxSize, fou, one);
	pVerts[vind + 3].set(+BoxSize, BoxSize, +BoxSize, fou, thr);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);

	// Bottom 
	vind += 4;
	tind += 2;
	pVerts[vind].set(+BoxSize, -BoxSize, +BoxSize, fou, fiv);
	pVerts[vind + 1].set(-BoxSize, -BoxSize, +BoxSize, fou, sev);
	pVerts[vind + 2].set(-BoxSize, -BoxSize, -BoxSize, two, sev);
	pVerts[vind + 3].set(+BoxSize, -BoxSize, -BoxSize, two, fiv);
	pTriList[tind].set(vind, vind + 2, vind + 1);
	pTriList[tind + 1].set(vind, vind + 3, vind + 2);


}

void ModelTools::CreateBigPyramid(StandardVertex *& pVerts, int & nverts, TriangleByIndex *& pTriList, int & ntri)
{
	float Size = 15;
	nverts = 16;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	Vect n;
	pVerts[0].set(+0.0f, -Size, +0.0f, 1, 0);
	pVerts[1].set(+Size, -Size, -Size, 0, 0);
	pVerts[2].set(-Size, -Size, +Size, 1, 1);
	pVerts[3].set(+Size, -Size, +Size, 0, 1);
	for (int i = 0; i <= 4; i++)
	{
		Vect n(0.0f, -1.0f, 0.0f);
		pVerts[i].SetNormal(n.getNorm());
	}

	pVerts[4].set(+0.0f, +Size, +0.0f, 0.5f, 0);
	pVerts[5].set(-Size, -Size, -Size, 1, 1);
	pVerts[6].set(+Size, -Size, -Size, 0, 1);
	for (int i = 4; i <= 6; i++)
	{
		Vect n(0.0f, 1.0f, -0.5f);
		pVerts[i].SetNormal(n.getNorm());
	}

	pVerts[7].set(+0.0f, +Size, +0.0f, 0.5f, 0);
	pVerts[8].set(-Size, -Size, +Size, 1, 1);
	pVerts[9].set(-Size, -Size, -Size, 0, 1);
	for (int i = 7; i <= 9; i++)
	{
		Vect n(-0.5f, 1.0f, -0.0f);
		pVerts[i].SetNormal(n.getNorm());
	}

	pVerts[10].set(+0.0f, +Size, +0.0f, 0.5f, 0);
	pVerts[11].set(+Size, -Size, -Size, 1, 1);
	pVerts[12].set(+Size, -Size, +Size, 0, 1);
	for (int i = 10; i <= 12; i++)
	{
		Vect n(+0.5f, 1.0f, 0.0f);
		pVerts[i].SetNormal(n.getNorm());
	}

	pVerts[13].set(+0.0f, +Size, +0.0f, 0.5f, 0);
	pVerts[14].set(-Size, -Size, +Size, 0, 1);
	pVerts[15].set(+Size, -Size, +Size, 1, 1);
	for (int i = 13; i <= 15; i++)
	{
		Vect n(0.0f, 1.0f, +0.5f);
		pVerts[i].SetNormal(n.getNorm());
	}
	// bottom face
	pTriList[0].set(2, 0, 1);
	pTriList[1].set(1, 3, 2);
	SetPlanarNormals(pVerts, 2, 0, 1);
	SetPlanarNormals(pVerts, 1, 3, 2);


	//TopFront
	pTriList[2].set(6, 5, 4);
	SetPlanarNormals(pVerts, 6, 5, 4);

	//TopRight
	pTriList[3].set(9, 8, 7);
	SetPlanarNormals(pVerts, 9, 8, 7);

	//Top Left
	pTriList[4].set(12, 11, 10);
	SetPlanarNormals(pVerts, 12, 11, 10);

	//Top Back
	pTriList[5].set(14, 15, 13);
	SetPlanarNormals(pVerts, 14, 15, 13);
}

StandardVertex ModelTools::GetCircleVert(int vi, int hi, int totalSides, int totalHeight)
{
	const float pi = 3.14159265358979f;
	float percent = ((float)vi / (float)totalHeight);
	float radians = 2 * pi * percent;
	float heightRatio = ((float)hi / totalSides);
	float height = (pi * heightRatio);

	StandardVertex returnVect;
	float x = (cosf(radians) / 2) * (sinf(height) * 2);
	float y = (sinf(radians) / 2) * (sinf(height) * 2);
	float z = cosf(height);
	float u = (float)vi;
	float v = (float)hi;
	Vect n = Vect(x, y, z, 0);
	returnVect.set(x, y, z, u, v, n);
	returnVect.SetNormal(n);
	return returnVect;

}

void ModelTools::CreateUnitSphere(int vslice, int hslice, StandardVertex *& pVerts, int & nverts, TriangleByIndex *& pTriList, int & ntri)
{
	nverts = (vslice + 1) * hslice;
	pVerts = new StandardVertex[nverts];
	ntri = 2 * ((vslice + 1) * (hslice - 1));
	pTriList = new TriangleByIndex[ntri];
	//int index[30][30];

	for (int vi = 0; vi < hslice; vi++)
	{
		for (int hi = 0; hi <= vslice; hi++)
		{
			StandardVertex vectIn = GetCircleVert(vi, hi, vslice, hslice);
			int index = (hi * hslice) + vi;
			pVerts[index] = vectIn;
			//index[hi] [vi] = 
		}
	}

	int middleVertex = ntri / 2;
	for (int i = 0; i < middleVertex; i++)
	{
		int indexOneRowDown = i + hslice;
		pTriList[i].set(i + 1, i, indexOneRowDown);
	}

	for (int i = 0; i < middleVertex; i++)
	{
		int indexOneRowDown = i + hslice;
		pTriList[i + middleVertex].set(i, indexOneRowDown - 1, indexOneRowDown);
	}

}

void ModelTools::CreateUnitPyramid(StandardVertex *& pVerts, int & nverts, TriangleByIndex *& pTriList, int & ntri)
{
	nverts = 16;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	pTriList = new TriangleByIndex[ntri];

	Vect n;
	pVerts[0].set(-0.5f, -0.5f, -0.5f, 1, 0);
	pVerts[1].set(+0.5f, -0.5f, -0.5f, 0, 0);
	pVerts[2].set(-0.5f, -0.5f, +0.5f, 1, 1);
	pVerts[3].set(+0.5f, -0.5f, +0.5f, 0, 1);
	for (int i = 0; i <= 4; i++)
	{
		Vect n(0.0f, -1.0f, 0.0f);
		pVerts[i].SetNormal(n.getNorm());
	}

	pVerts[4].set(+0.0f, +0.5f, +0.0f, 0.5f, 0);
	pVerts[5].set(-0.5f, -0.5f, -0.5f, 1, 1);
	pVerts[6].set(+0.5f, -0.5f, -0.5f, 0, 1);
	for (int i = 4; i <= 6; i++)
	{
		Vect n(0.0f, 1.0f, -0.5f);
		pVerts[i].SetNormal(n.getNorm());
	}

	pVerts[7].set(+0.0f, +0.5f, +0.0f, 0.5f, 0);
	pVerts[8].set(-0.5f, -0.5f, +0.5f, 1, 1);
	pVerts[9].set(-0.5f, -0.5f, -0.5f, 0, 1);
	for (int i = 7; i <= 9; i++)
	{
		Vect n(-0.5f, 1.0f, -0.0f);
		pVerts[i].SetNormal(n.getNorm());
	}

	pVerts[10].set(+0.0f, +0.5f, +0.0f, 0.5f, 0);
	pVerts[11].set(+0.5f, -0.5f, -0.5f, 1, 1);
	pVerts[12].set(+0.5f, -0.5f, +0.5f, 0, 1);
	for (int i = 10; i <= 12; i++)
	{
		Vect n(+0.5f, 1.0f, 0.0f);
		pVerts[i].SetNormal(n.getNorm());
	}

	pVerts[13].set(+0.0f, +0.5f, +0.0f, 0.5f, 0);
	pVerts[14].set(-0.5f, -0.5f, +0.5f, 0, 1);
	pVerts[15].set(+0.5f, -0.5f, +0.5f, 1, 1);
	for (int i = 13; i <= 15; i++)
	{
		Vect n(0.0f, 1.0f, +0.5f);
		pVerts[i].SetNormal(n.getNorm());
	}
	// bottom face
	pTriList[0].set(2, 0, 1);
	pTriList[1].set(1, 3, 2);
	SetPlanarNormals(pVerts, 2, 0, 1);
	SetPlanarNormals(pVerts, 1, 3, 2);


	//TopFront
	pTriList[2].set(6, 5, 4);
	SetPlanarNormals(pVerts, 6, 5, 4);

	//TopRight
	pTriList[3].set(9, 8, 7);
	SetPlanarNormals(pVerts, 9, 8, 7);

	//Top Left
	pTriList[4].set(12, 11, 10);
	SetPlanarNormals(pVerts, 12, 11, 10);

	//Top Back
	pTriList[5].set(14, 15, 13);
	SetPlanarNormals(pVerts, 14, 15, 13);





}


