#include "DXApp.h"
#include <windows.h>
#include <sstream>
#include <assert.h>
#include "d3dUtil.h"

// needed to load shaders from file
#include <d3dcompiler.h>

#include "Model.h"
#include "ModelTools.h"
#include "TerrainModel.h"
#include "DirectXTex.h"
#include "Texture.h"

DXApp::DXApp(HWND hwnd)
{
	assert(hwnd);
	mhMainWnd = hwnd;

	BackgroundColor = Colors::MidnightBlue;

	md3dDevice = nullptr;
	md3dImmediateContext = nullptr;
	mSwapChain = nullptr;
	mRenderTargetView = nullptr;

	// Get window data through the window handle
	RECT rc;
	BOOL err = GetClientRect(mhMainWnd, &rc);  // Seriously MS: Redifining BOOL as int? Confusing much?
	assert(err);

	// get width/hight
	mClientWidth = rc.right - rc.left;
	mClientHeight = rc.bottom - rc.top;

	// Get window caption
	const int MAX_LABEL_LENGTH = 100; // probably overkill...
	WCHAR str[MAX_LABEL_LENGTH];
	GetWindowText(mhMainWnd, str, MAX_LABEL_LENGTH);
	mMainWndCaption = str;

	// Initialize DX11
	this->InitDirect3D();

	// Demo initialization
	this->InitDemo();
}

DXApp::~DXApp()
{
	delete ShaderMan;

	ReleaseAndDeleteCOMobject(MarkMirrorDSS);
	ReleaseAndDeleteCOMobject(DrawReflectionDSS);
	ReleaseAndDeleteCOMobject(FrontFaceAsCCWRS);
	ReleaseAndDeleteCOMobject(MirrorFrontFaceAsClockWiseRS);
	ReleaseAndDeleteCOMobject(NoWriteToRenderTargetBS);
	ReleaseAndDeleteCOMobject(TransparentBS);

	ReleaseAndDeleteCOMobject(md3dImmediateContext);
	ReleaseAndDeleteCOMobject(mSwapChain);
	ReleaseAndDeleteCOMobject(mRenderTargetView);
	ReleaseAndDeleteCOMobject(mpDepthStencilView);

	// Must be done BEFORE the device is released
	ReportLiveDXObjects();		// See http://masterkenth.com/directx-leak-debugging/

	ReleaseAndDeleteCOMobject(md3dDevice);
}

// See http://masterkenth.com/directx-leak-debugging/
void DXApp::ReportLiveDXObjects()
{
#ifdef _DEBUG
	HRESULT hr = S_OK;

	// Now we set up the Debug interface, to be queried on shutdown
	ID3D11Debug* debugDev;
	hr = md3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));

	debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	ReleaseAndDeleteCOMobject(debugDev);
#endif
}

void DXApp::InitDirect3D()
{
	HRESULT hr = S_OK;

	// Controls MSAA option:
	// - 4x count level garanteed for all DX11 
	// - MUST be the same for depth buffer!
	// - We _need_ to work with the depth buffer because reasons... (see below)
	DXGI_SAMPLE_DESC sampDesc;
	sampDesc.Count = 1;
	sampDesc.Quality = static_cast<UINT>(D3D11_CENTER_MULTISAMPLE_PATTERN);  // MS: what's with the type mismtach?

	hr = Com_Manager::CreateSwapChain(md3dDevice, md3dImmediateContext, mSwapChain, mhMainWnd, sampDesc);
	assert(SUCCEEDED(hr));

	// Create a render target view		https://msdn.microsoft.com/en-us/library/windows/desktop/ff476582(v=vs.85).aspx
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	assert(SUCCEEDED(hr));

	hr = md3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
	ReleaseAndDeleteCOMobject(pBackBuffer);
	assert(SUCCEEDED(hr));

	// First we fix what it means for triangles to be front facing.
	// Requires setting a whole new rasterizer state
	hr = Com_Manager::RasterizeDesc(md3dDevice, FrontFaceAsCCWRS);
	assert(SUCCEEDED(hr));
	md3dImmediateContext->RSSetState(FrontFaceAsCCWRS);

	Com_Manager::Setup(md3dDevice, md3dImmediateContext, sampDesc, mpDepthStencilView, mClientWidth, mClientHeight);

	//md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);  // to use without depth stencil
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mpDepthStencilView);


}

void DXApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((mTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void DXApp::FrameTick()
{
	mTimer.Tick();
	CalculateFrameStats();

	this->UpdateScene();
	this->DrawScene();
}

void DXApp::InitDemo()
{
	HRESULT hr = S_OK;

	hr = Com_Manager::MarkMirrorDesc(md3dDevice, MarkMirrorDSS);
	assert(SUCCEEDED(hr));

	hr = Com_Manager::DrawReflectionDesc(md3dDevice, DrawReflectionDSS);
	assert(SUCCEEDED(hr));

	hr = Com_Manager::RasterizeDesc_Mirror(md3dDevice, MirrorFrontFaceAsClockWiseRS);
	assert(SUCCEEDED(hr));

	hr = Com_Manager::StandardBlendDesc(md3dDevice, TransparentBS);
	assert(SUCCEEDED(hr));

	hr = Com_Manager::NoWriteBlendDesc(md3dDevice, NoWriteToRenderTargetBS);
	assert(SUCCEEDED(hr));
	/******************************************/


	ShaderMan = new Shader_Manager(md3dDevice, md3dImmediateContext);
	Ticker = 0;

	Vect PL_Position00 = Vect(+4.0f, 3.0f, +3.5f, 1);
	Vect PL_Position01 = Vect(+5.0f, 7.0f, -2.0f, 1);
	Vect PL_Position02 = Vect(-4.0f, 12.0f, +3.0f, 1);
	Vect PL_Position03 = Vect(-5.0f, 9.0f, -2.5f, 1);

	Vect PL_Color01 = Vect(0.0f, 1.0f, 0.0f, 0.0f);
	Vect PL_Color04 = Vect(1.0f, 0.8f, 0.2f, 0.0f);
	Vect PL_ColorOf = Vect(0.0f, 0.0f, 0.0f, 0.0f);
	PointLight_ToShare Light00(PL_Position00, PL_ColorOf);
	PointLight_ToShare Light01(PL_Position01, PL_ColorOf);
	PointLight_ToShare Light02(PL_Position02, PL_ColorOf);
	PointLight_ToShare Light03(PL_Position03, PL_ColorOf);

	PointLight_ToShare arrayIn[LightCount::PLCount] = { Light00, Light01, Light02, Light03 };
	ShaderMan->SetLights(arrayIn);

	// Initialize the projection matrix
	mCam.setPerspective(45, mClientWidth / (float)mClientHeight, 1.0f, 1000.0f);

	// Initialize gimmicky mouse control values
	mTheta = .5;
	mPhi = 3.1415f / 8;
	mRadius = 30;

	mTimer.Reset();
}

void DXApp::UpdateScene()
{
	//mWorld *= Matrix(ROT_Y, .0001f);  // Simple slow rotation about y axis

	// Build the view matrix using gimmicky trick
	Vect target = Vect(0, 0, 0, 0);
	Vect up = Vect(0, 1, 0, 0);
	Vect pos = Vect(0, 30, -mRadius) * Matrix(ROT_Y, mTheta) * Matrix(ROT_X, mPhi);
	mCam.setOrientAndPosition(up, target, pos);

	mCam.updateCamera();
}

void DXApp::RenderInFrontOfMirror()
{
	ShaderMan->RenderSkyBox();
	ShaderMan->RenderTextLight();
	ShaderMan->RenderColorLight();
}

void DXApp::GetMirrorStencil()
{
	md3dImmediateContext->OMSetBlendState(NoWriteToRenderTargetBS, nullptr, 0xffffffff);
	// STENCIL: Set up the stencil for marking ('1' for all pixels that passed the test. See comment at line 35)
	md3dImmediateContext->OMSetDepthStencilState(MarkMirrorDSS, 1);

	// Render the mirror
	ShaderMan->RenderMirror();

	// STENCIL: stop using the stencil
	md3dImmediateContext->OMSetDepthStencilState(0, 0);
	// BLEND STATE: Return the blend state to normal (writing to render target)
	md3dImmediateContext->OMSetBlendState(0, nullptr, 0xffffffff);
}

void DXApp::RenderReflections()
{
	// WINDINGS: face winding will appear inside out after reflection. Switching to CW front facing
	md3dImmediateContext->RSSetState(MirrorFrontFaceAsClockWiseRS);
	// STENCIL: Use the stencil test (reference value 1) and only pass the test if the stencil already had a one present
	md3dImmediateContext->OMSetDepthStencilState(DrawReflectionDSS, 1);

	//* Reflected objects (same worldmat, but multiplied by mirror's reflection matrix)
	Vect eyepos;
	mCam.getPos(eyepos);
	ShaderMan->RenderTextLight_Reflection();
	ShaderMan->RenderColorLight_Reflection();

	// STENCIL: Stop using the stencil
	md3dImmediateContext->OMSetDepthStencilState(0, 0);
	// WINDING: back to normal windings
	md3dImmediateContext->RSSetState(FrontFaceAsCCWRS);
	//*/
}

void DXApp::RenderMirrorWithTransparency()
{
	//*
	// BLENDING: Sets standard alpha blending: c_src*(a_src) + c_dst*(1-a_src)
	md3dImmediateContext->OMSetBlendState(TransparentBS, nullptr, 0xffffffff);

	ShaderMan->RenderMirror();

	// BLENDING: Back to normal blending (turn off transparent blending)
	md3dImmediateContext->OMSetBlendState(0, nullptr, 0xffffffff);
	//*/
}

void DXApp::RenderObjectsBehindMirror()
{
	ShaderMan->RenderBehindMirror();
}
void DXApp::DrawScene()
{
	// Clear the back buffer
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, VasA(BackgroundColor));
	// Clear the depth stencil;
	md3dImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// ------- Camera data can be sent only once
	Ticker += mTimer.DeltaTime();
	if (Ticker > 0.01f)
	{
		ShaderMan->UpdateLightPos();
		Ticker = 0.0f;
	}


	ShaderMan->SendCam(mCam);

	RenderInFrontOfMirror();
	GetMirrorStencil();
	RenderReflections();
	RenderMirrorWithTransparency();
	RenderObjectsBehindMirror();

	// Switches the display to show the now-finished back-buffer
	mSwapChain->Present(0, 0);
}

void DXApp::OnMouseDown(WPARAM btnState, int xval, int yval)
{
	// Gimmicky mouse control
	if (btnState & MK_LBUTTON)
	{
		MousePos[x] = static_cast<float>(xval);
		MousePos[y] = static_cast<float>(yval);
	}
}

void DXApp::OnMouseUp(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseMove(WPARAM btnState, int xval, int yval)
{
	// Gimmicky mouse control
	if (btnState & MK_LBUTTON)
	{
		float dx = 0.01f*(MousePos[x] - xval);
		float dy = 0.01f*(MousePos[y] - yval);

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi -= dy;

		static float verticalMax = MATH_PI2 - 1.0f;

		if (mPhi > verticalMax)
			mPhi = verticalMax;
		else if (mPhi < -verticalMax)
			mPhi = -verticalMax;
	}

	MousePos[x] = static_cast<float>(xval);
	MousePos[y] = static_cast<float>(yval);
}
