#ifndef DX_Starter
#define DX_Starter

#include <d3d11.h>
#include "d3dUtil.h"

class DXStarter
{
public:
	static void Starter(ID3D11Device *& md3dDevice, ID3D11DeviceContext *& md3dImmediateContext, HWND mhMainWnd, IDXGISwapChain*& mSwapChain,
		ID3D11RenderTargetView*& mRenderTargetView, int mClientWidth, int mClientHeight, ID3D11DepthStencilView*& mpDepthStencilView);
};



#endif // !DX_Starter



