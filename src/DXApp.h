// DXApp
// Andre Berthiaume, June 2016
// Note: DX 11 SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812
// Note on weird stuff with swap chain (1s and 2s) https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

#ifndef _DXApp
#define _DXApp

#include <d3d11.h>
#include "d3dUtil.h"
#include "Align16.h"
#include "GameTimer.h"
#include <string>

// New includes for demo
#include "Vect.h"
#include "Matrix.h"
#include "Camera.h"
#include "ShaderTextureLight.h"
#include "ShaderColorLight.h"
#include "GraphicObject_TextureLight.h"
#include "GraphicObject_ColorLight.h"
#include "GraphicObject_Color.h"
#include "Shader_Manager.h"
#include "Com_Manager.h"

class Model;
class TerrainModel;
class Texture;

class DXApp : public Align16
{
private:
	// Main window handle
	HWND      mhMainWnd;

	Vect BackgroundColor;

	// DX application elements
	ID3D11Device* md3dDevice;					// Connects to the graphics card
	ID3D11DeviceContext* md3dImmediateContext;	// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations

	GameTimer mTimer;
	double Ticker;
	std::wstring mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	void InitDirect3D();
	void InitDemo();
	void UpdateScene();
	void RenderInFrontOfMirror();
	void GetMirrorStencil();
	void RenderReflections();
	void RenderMirrorWithTransparency();
	void RenderObjectsBehindMirror();
	void DrawScene();
	void CalculateFrameStats();

	// Debug utility
	void ReportLiveDXObjects();

	/*****************************************/
	// Demo specific additions
	Shader_Manager *ShaderMan;

	Camera mCam;

	ID3D11DepthStencilState* MarkMirrorDSS;
	ID3D11DepthStencilState* DrawReflectionDSS;

	ID3D11RasterizerState* FrontFaceAsCCWRS;
	ID3D11RasterizerState* MirrorFrontFaceAsClockWiseRS;

	ID3D11BlendState* NoWriteToRenderTargetBS;
	ID3D11BlendState* TransparentBS;

	/**************************************/

	// gimmicky mouse control
	Vect MousePos;
	float mTheta;
	float mPhi;
	float mRadius;



public:
	DXApp(HWND hwnd);
	virtual ~DXApp();

	void FrameTick();

	// overrides for handling mouse input.
	void OnMouseDown(WPARAM btnState, int xval, int yval);
	void OnMouseUp(WPARAM btnState, int xval, int yval);
	void OnMouseMove(WPARAM btnState, int xval, int yval);


};

#endif _DXApp