#ifndef D3D11GRAPHICS_H
#define D3D11GRAPHICS_H

#include "dxgi.h"
#include "d3d11.h"
#include "d3dcommon.h"
#include "DirectXMath.h"
#include "d3dx11effect.h"

#define FULL_SCREEN false;
#define VSYNC_ENABLE true;
#define FAR_POINT 1.0f;
#define NEAR_POINT 0.0f;

//Some Macros
#define SafeRelease(pCOM) { if (pCOM != NULL) { pCOM->Release(); pCOM = NULL; } }
#define SafeDelete(p) { if (p != NULL) {delete p; p = NULL; } }
#define CheckHR(hResult, e) { if (hResult != S_OK) {throw e; } }


struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};



class D3D11Graphics
{
public: 
	D3D11Graphics(HWND hwnd);
	~D3D11Graphics();
	void Render();

private:
	//Normal Variables
	float windowWidth_;
	float windowHeight_;

	//D3DX11
	D3D_FEATURE_LEVEL d3d_feature_level_;
	ID3D11Device* device_;
	ID3D11DeviceContext* deviceContext_;
	UINT supportNumMSQualityLevel_;
	IDXGISwapChain* swapChain_;
	ID3D11RenderTargetView* renderTargetView_;
	ID3D11Texture2D* stencilBuffer_;
	ID3D11DepthStencilView* depthStencilView_;

	ID3DX11Effect* effect_;
	ID3DX11EffectTechnique* effectTechnique_;
	void initializeEffect_();

	ID3D11InputLayout* inputLayout_;
	void initializeInputLayout_();

	ID3D11Buffer* vertexBuffer_;
	void initializeVertexBuffer_();

	ID3D11Buffer* indexBuffer_;
	void initializeIndexBuffer_();

	DirectX::XMFLOAT4X4 mWorld_;
	DirectX::XMFLOAT4X4 mView_;
	DirectX::XMFLOAT4X4 mProjection_;
	DirectX::XMMATRIX mWVP_;
	void initializeMatrix_();

};


#endif
