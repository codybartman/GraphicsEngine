#ifndef _Com_Manager
#define _Com_Manager

#include <d3d11.h>
#include "d3dUtil.h"

class Com_Manager
{
public:
	Com_Manager();
	~Com_Manager();

	static void Setup(ID3D11Device *& md3dDevice, ID3D11DeviceContext *& md3dImmediateContext, DXGI_SAMPLE_DESC sampDesc, ID3D11DepthStencilView *& mpDepthStencilView, int mClientWidth, int mClientHeight);

	static HRESULT CreateSwapChain(ID3D11Device *&md3dDevice, ID3D11DeviceContext*& md3dImmediateContext, IDXGISwapChain *&mSwapChain, HWND mhMainWnd, DXGI_SAMPLE_DESC sampDesc);

	static HRESULT RasterizeDesc(ID3D11Device * md3dDevice, ID3D11RasterizerState *& FrontFaceAsCCWRS);

	static HRESULT StandardBlendDesc(ID3D11Device * md3dDevice, ID3D11BlendState *& TransparentBS);

	static HRESULT NoWriteBlendDesc(ID3D11Device * md3dDevice, ID3D11BlendState *& NoWriteToRenderTargetBS);

	static HRESULT RasterizeDesc_Mirror(ID3D11Device * md3dDevice, ID3D11RasterizerState *& MirrorFrontFaceAsClockWiseRS);

	static HRESULT MarkMirrorDesc(ID3D11Device * md3dDevice, ID3D11DepthStencilState *& MarkMirrorDSS);

	static HRESULT DrawReflectionDesc(ID3D11Device * md3dDevice, ID3D11DepthStencilState *& DrawReflectionDSS);



private:
	static D3D11_RASTERIZER_DESC BasicRasterizerDesc();

	static D3D11_BLEND_DESC BasicBlendDesc();

	static D3D11_DEPTH_STENCIL_DESC BasicDesc();


};




#endif // !_Com_Manager







