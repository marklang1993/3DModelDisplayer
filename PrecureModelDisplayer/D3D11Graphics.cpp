#include "D3D11Graphics.h"
#include "PrecureModelDisplayerException.h"
#include "Library.h"

#include <vector>
#include <fstream>

using namespace DirectX;

D3D11Graphics::D3D11Graphics(HWND hwnd)
{
	//Set all Pointers to NULL
	device_ = NULL;
	deviceContext_= NULL;
	swapChain_ = NULL;
	renderTargetView_ = NULL;
	stencilBuffer_ = NULL;
	depthStencilView_ = NULL;

	effect_ = NULL;
	effectTechnique_ = NULL;

	inputLayout_ = NULL;
	vertexBuffer_ = NULL;
	indexBuffer_ = NULL;


	//Return Value
	HRESULT hResult = NULL;

	//1. Create D3D11 Device & DeviceContext
	hResult = D3D11CreateDevice(
		NULL,									//Default Adapter
		D3D_DRIVER_TYPE_HARDWARE,				//Hardware Accleration
		NULL,									//Hardware is chosen
		0,										//No Optional Flags

		//D3D11_CREATE_DEVICE_SINGLETHREADED,		//Single Thread Mode is chosen 

		NULL,									//Array of Available D3D Feature Levels
		0,										//Size of the above Array
		D3D11_SDK_VERSION,						//Fixed Value
		&device_,								//Pointer to the pointer pointed to a D3D11 Device
		&d3d_feature_level_,					//Selected D3D Feature Level
		&deviceContext_							//Pointer to the pointer pointed to a D3D11 DeviceContext
		);
	//Check the Result of Creating D3D11 Device
	CheckHR(hResult, PMD_FailCreateD3D11DeviceException());


	//2. CheckMultiSampling
	hResult = device_->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		4,								//4x
		&supportNumMSQualityLevel_		//Supported Number of Multisampling Quality Level
		);
	if (hResult != S_OK)
	{
		//If checking failed, set it 0 (Unsupported).
		supportNumMSQualityLevel_ = 0;
	}


	//3. Create SwapChain
	RECT window_rect;
	if (GetWindowRect(hwnd, &window_rect) == FALSE)
	{
		SafeRelease(device_);
		SafeRelease(deviceContext_);
		throw PMD_FailCreateSwapChainException();
	}

	DXGI_MODE_DESC mode_desc;
	mode_desc.Width = window_rect.right - window_rect.left;
	mode_desc.Height = window_rect.bottom - window_rect.top;
	mode_desc.RefreshRate.Numerator = 60; //60Hz
	mode_desc.RefreshRate.Denominator = 1;
	mode_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	mode_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	mode_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	swap_chain_desc.BufferDesc = mode_desc;
	swap_chain_desc.BufferCount = 1; //only 1 Buffer used
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //used for render target output
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_desc.OutputWindow = hwnd;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Windowed = !FULL_SCREEN;
	//MultiSampling
	if (supportNumMSQualityLevel_ == 0)
	{
		//MultiSampling is disable
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
	}
	else
	{
		swap_chain_desc.SampleDesc.Count = 4; //4 sampling point
		swap_chain_desc.SampleDesc.Quality = supportNumMSQualityLevel_ - 1;
	}

	IDXGIDevice* pIDXGIDevice = NULL;
	IDXGIAdapter* pIDXGIAdapter = NULL;
	IDXGIFactory* pIDXGIFactory = NULL;
	hResult = device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pIDXGIDevice));
	CheckHR(hResult, PMD_FailCreateSwapChainException());
	hResult = pIDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pIDXGIAdapter));
	CheckHR(hResult, PMD_FailCreateSwapChainException());
	hResult = pIDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pIDXGIFactory));
	CheckHR(hResult, PMD_FailCreateSwapChainException());
	hResult = pIDXGIFactory->CreateSwapChain(device_, &swap_chain_desc, &swapChain_);
	CheckHR(hResult, PMD_FailCreateSwapChainException());

	SafeRelease(pIDXGIDevice);
	SafeRelease(pIDXGIAdapter);
	SafeRelease(pIDXGIFactory);


	//4. Create RenderTargetView
	ID3D11Texture2D* backBuffer;
	hResult = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**> (&backBuffer)); //GetBuffer() will increase the COM reference count
	CheckHR(hResult, PMD_FailCreateRenderTargetViewException());
	hResult = device_->CreateRenderTargetView(backBuffer, NULL, &renderTargetView_);
	CheckHR(hResult, PMD_FailCreateRenderTargetViewException());
	//Release
	SafeRelease(backBuffer); //Decrease the COM reference count


	//5. Create DepthStencilView
	D3D11_TEXTURE2D_DESC texture2d_desc;
	texture2d_desc.ArraySize = 1;							//Used for Texture Array. NOT useful here
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//Binding type
	texture2d_desc.CPUAccessFlags = 0;						//No access for CPU
	texture2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  //24bits for Depth, 8 bits for template
	texture2d_desc.Height = window_rect.bottom - window_rect.top;
	texture2d_desc.Width = window_rect.right - window_rect.left;
	texture2d_desc.MipLevels = 1;							//Not use Mipmap
	texture2d_desc.MiscFlags = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;				//For GPU Read & Write
	//MultiSampling
	if (supportNumMSQualityLevel_ == 0)
	{
		//MultiSampling is disable
		texture2d_desc.SampleDesc.Count = 1;
		texture2d_desc.SampleDesc.Quality = 0;
	}
	else
	{
		texture2d_desc.SampleDesc.Count = 4; //4 sampling point
		texture2d_desc.SampleDesc.Quality = supportNumMSQualityLevel_ - 1;
	}
	
	hResult = device_->CreateTexture2D(&texture2d_desc, NULL, &stencilBuffer_);
	CheckHR(hResult, PMD_FailCreateDepthStencilViewException());
	hResult = device_->CreateDepthStencilView(stencilBuffer_, NULL, &depthStencilView_);
	CheckHR(hResult, PMD_FailCreateDepthStencilViewException());
	//Bind to Rendering Pipeline
	deviceContext_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);


	//6. Set ViewPoint
	D3D11_VIEWPORT viewpoint;
	//#####WARNNING
	windowWidth_ = static_cast<FLOAT>(window_rect.right - window_rect.left);
	windowHeight_ = static_cast<FLOAT>(window_rect.bottom - window_rect.top);
	
	viewpoint.Width = windowWidth_;
	viewpoint.Height = windowHeight_;
	viewpoint.TopLeftX = 0.0f;
	viewpoint.TopLeftY = 0.0f;
	viewpoint.MinDepth = NEAR_POINT;
	viewpoint.MaxDepth = FAR_POINT;

	deviceContext_->RSSetViewports(1, &viewpoint);

	//7. Initialize Effect
	initializeEffect_();

	//8. Initialize InputLayout
	initializeInputLayout_();

	//9. Initialize Buffer
	initializeVertexBuffer_();
	initializeIndexBuffer_();

	//10. Initilize Matrix
	initializeMatrix_();
}

D3D11Graphics::~D3D11Graphics()
{
	SafeRelease(device_);
	SafeRelease(deviceContext_);
	SafeRelease(swapChain_);
	SafeRelease(renderTargetView_);
	SafeRelease(stencilBuffer_);
	SafeRelease(depthStencilView_);

	SafeRelease(effect_);
	//SafeRelease(effectTechnique_);

	SafeRelease(inputLayout_);
	SafeRelease(vertexBuffer_);
	SafeRelease(indexBuffer_);
}

void D3D11Graphics::Render()
{
	//Clear Screen
	deviceContext_->ClearRenderTargetView(renderTargetView_, reinterpret_cast<float*>(&Colors::White));
	deviceContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	//Calculate Matrix WVP
	mWVP_ = XMLoadFloat4x4(&mWorld_) * XMLoadFloat4x4(&mView_) * XMLoadFloat4x4(&mProjection_);
	//Bind Vertex Buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	//Bind Index Buffer
	deviceContext_->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//Set InputLayout
	deviceContext_->IASetInputLayout(inputLayout_);
	//Set Topology
	deviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Set WVP Matrix
	ID3DX11EffectMatrixVariable* effectMatrixVariable;
	effectMatrixVariable = effect_->GetVariableByName("matrixWVP")->AsMatrix();
	effectMatrixVariable->SetMatrix(reinterpret_cast<float*>(&mWVP_));
	SafeRelease(effectMatrixVariable);

	D3DX11_TECHNIQUE_DESC techDesc;
	effectTechnique_->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		effectTechnique_->GetPassByIndex(p)->Apply(0, deviceContext_);
		deviceContext_->DrawIndexed(12, 0, 0);
	}

	swapChain_->Present(0, 0);
}



void D3D11Graphics::initializeEffect_()
{
	using namespace std;
	
	//Read fxo File
	ifstream fxo;
	fxo.open("HLSL\\Shader.fxo", ios_base::binary);
	//If fxo file failed to open
	if (fxo.fail())
	{
		throw PMD_FailedInitializeEffect();
	}
	fxo.seekg(0, std::ios_base::end);
	int length = (int)fxo.tellg();
	fxo.seekg(0, std::ios_base::beg);

	vector<char> fxo_binary(length);
	fxo.read(&fxo_binary[0], length);

	CheckHR(D3DX11CreateEffectFromMemory(&fxo_binary[0], length, 0, device_, &effect_), PMD_FailedInitializeEffect());

	effectTechnique_ = effect_->GetTechniqueByName("Tech");		//Corresponding to Shader.fx
}

void D3D11Graphics::initializeInputLayout_()
{
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	effectTechnique_->GetPassByIndex(0)->GetDesc(&passDesc);

	CheckHR(device_->CreateInputLayout(inputElementDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &inputLayout_), PMD_FailedCreateInputLayout())

	//deviceContext_->IASetInputLayout(inputLayout_);
}

void D3D11Graphics::initializeVertexBuffer_()
{
	//Defein Vertex
	Vertex vertex[]=
	{
		/*{ XMFLOAT3(1.0f, 0.0f, 0.0f), *(const XMFLOAT4*)&Colors::Red },
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), *(const XMFLOAT4*)&Colors::Green },
		{ XMFLOAT3(0.0f, 1.0f, 0.0f), *(const XMFLOAT4*)&Colors::Blue },
		{ XMFLOAT3(0.0f, 0.0f, 0.1f), *(const XMFLOAT4*)&Colors::Yellow }*/

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), *(const XMFLOAT4*)&Colors::White },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), *(const XMFLOAT4*)&Colors::Black },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), *(const XMFLOAT4*)&Colors::Red },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), *(const XMFLOAT4*)&Colors::Green },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), *(const XMFLOAT4*)&Colors::Blue },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), *(const XMFLOAT4*)&Colors::Yellow },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), *(const XMFLOAT4*)&Colors::Red },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), *(const XMFLOAT4*)&Colors::Green }
	};

	//Create Vertex Buffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(Vertex) * 8;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = vertex;
	subresourceData.SysMemPitch = 0;
	subresourceData.SysMemSlicePitch = 0;

	CheckHR(device_->CreateBuffer(&bufferDesc, &subresourceData, &vertexBuffer_), PMD_FailedCreateBuffer());

	////Bind Vertex Buffer
	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;
	//deviceContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
}

void D3D11Graphics::initializeIndexBuffer_()
{
	//Define Index
	UINT index[] =
	{
		/*0, 1, 2,
		0, 1, 3,
		0, 3, 2,
		1, 2, 3*/

		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	
	//Create Index Buffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(UINT) * 36;
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = index;
	subresourceData.SysMemPitch = 0;
	subresourceData.SysMemSlicePitch = 0;

	CheckHR(device_->CreateBuffer(&bufferDesc, &subresourceData, &indexBuffer_), PMD_FailedCreateBuffer());

	////Bind Index Buffer
	//deviceContext_->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
}

void D3D11Graphics::initializeMatrix_()
{
	//World
	XMMATRIX mIdentity = XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&mWorld_, mIdentity);

	//View
	XMVECTOR eyePosition = XMVectorSet(5.0f, 3.0f, -10.0f, 1.0f);
	XMVECTOR focusPoint = XMVectorZero();
	XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX mView = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);
	DirectX::XMStoreFloat4x4(&mView_, mView);

	//Projection
	XMMATRIX mProjection = XMMatrixPerspectiveFovLH(0.25f * PI, windowWidth_ / windowHeight_, 1.0f, 1000.0f);
	DirectX::XMStoreFloat4x4(&mProjection_, mProjection);
	
	////Calculate Matrix WVP
	//mWVP_ = XMLoadFloat4x4(&mWorld_) * XMLoadFloat4x4(&mView_) * XMLoadFloat4x4(&mProjection_);
}