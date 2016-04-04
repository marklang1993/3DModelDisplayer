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

	void getCameraPos(int &rCamera, int &alphaCamera, int &betaCamera);
	void setCameraPos(int rCamera, int alphaCamera, int betaCamera);

private:
	//Window Size
	float windowWidth_;
	float windowHeight_;

	//D3DX11 Basic
	D3D_FEATURE_LEVEL d3d_feature_level_;
	ID3D11Device* device_;
	ID3D11DeviceContext* deviceContext_;
	UINT supportNumMSQualityLevel_;
	IDXGISwapChain* swapChain_;
	ID3D11RenderTargetView* renderTargetView_;
	ID3D11Texture2D* stencilBuffer_;
	ID3D11DepthStencilView* depthStencilView_;

	//D3DX11 Effect
	ID3DX11Effect* effect_;
	ID3DX11EffectTechnique* effectTechnique_;
	void initializeEffect_();

	//Input Layout
	ID3D11InputLayout* inputLayout_;
	void initializeInputLayout_();

	//Vertex Buffer
	ID3D11Buffer* vertexBuffer_;
	void initializeVertexBuffer_();

	//Index Buffer
	ID3D11Buffer* indexBuffer_;
	void initializeIndexBuffer_();

	//Coordinates System Transform
	DirectX::XMFLOAT4X4 mWorld_;			//Matrix of World Transformation
	DirectX::XMFLOAT4X4 mView_;				//Matrix of View Transformation
	DirectX::XMFLOAT4X4 mProjection_;		//Matrix of Projection Transformation
	DirectX::XMMATRIX mWVP_;		
	int rCamera_;							//Radius of Camera
	double alphaCamera_;					//Alpha angle of Camera(Radian)
	double betaCamera_;						//Beta angle of Camera(Radian)
	void initializeMatrix_();

};


#endif
