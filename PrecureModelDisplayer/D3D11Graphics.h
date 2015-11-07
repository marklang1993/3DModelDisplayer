#ifndef D3D11GRAPHICS_H
#define D3D11GRAPHICS_H

#include "dxgi.h"
#include "d3d11.h"
#include "d3dcommon.h"
#include "DirectXMath.h"

#define FULL_SCREEN false;
#define VSYNC_ENABLE true;
#define FAR_POINT 1.0f;
#define NEAR_POINT 0.0f;

//Some Macros
#define SafeRelease(pCOM) { if (pCOM != NULL) { pCOM->Release(); pCOM = NULL; } }
#define SafeDelete(p) { if (p != NULL) {delete p; p = NULL; } }
#define CheckHR(hResult, e) { if (hResult != S_OK) {throw e; } }

class D3D11Graphics
{
public: 
	D3D11Graphics(HWND hwnd);
	~D3D11Graphics();
	void Render();

	//static void SafeRelease(IUnknown* pObject);

private:
	D3D_FEATURE_LEVEL d3d_feature_level_;
	ID3D11Device* device_;
	ID3D11DeviceContext* deviceContext_;
	UINT supportNumMSQualityLevel_;
	IDXGISwapChain* swapChain_;
	ID3D11RenderTargetView* renderTargetView_;
	ID3D11Texture2D* stencilBuffer_;
	ID3D11DepthStencilView* depthStencilView_;
};


#endif
