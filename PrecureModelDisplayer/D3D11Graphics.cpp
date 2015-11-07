#include "D3D11Graphics.h"
#include "PrecureModelDisplayerException.h"

D3D11Graphics::D3D11Graphics(HWND hwnd)
{
	//Set all Pointers to NULL
	device_ = NULL;
	deviceContext_= NULL;
	swapChain_ = NULL;
	renderTargetView_ = NULL;
	stencilBuffer_ = NULL;
	depthStencilView_ = NULL;
	//Return Value
	HRESULT hResult = NULL;

	//Initialize D3D_FEATURE_LEVEL
	D3D_FEATURE_LEVEL* d3d_feature_levels = new D3D_FEATURE_LEVEL[3];
	d3d_feature_levels[0] = D3D_FEATURE_LEVEL_9_1;
	d3d_feature_levels[1] = D3D_FEATURE_LEVEL_10_0;
	d3d_feature_levels[2] = D3D_FEATURE_LEVEL_11_0;
	//Create D3D11 Device & DeviceContext, Choose Feature Level
	hResult = D3D11CreateDevice(
		NULL,									//Default Adapter
		D3D_DRIVER_TYPE_HARDWARE,				//Hardware Accleration
		NULL,									//Hardware is chosen
		D3D11_CREATE_DEVICE_SINGLETHREADED,		//Single Thread Mode is chosen
		d3d_feature_levels,						//Array of Available D3D Feature Levels
		3,										//Size of the above Array
		D3D11_SDK_VERSION,						//Fixed Value
		&device_,								//Pointer to the pointer pointed to a D3D11 Device
		&d3d_feature_level_,					//Selected D3D Feature Level
		&deviceContext_							//Pointer to the pointer pointed to a D3D11 DeviceContext
		);
	//release the array of D3D_FEATURE_LEVEL
	delete[] d3d_feature_levels;
	//Check the Result of Creating D3D11 Device
	if (hResult != S_OK)
	{
		throw PMD_FailCreateD3D11DeviceException();
	}

	//CheckMultiSampling
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

	//Create SwapChain
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
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_desc.OutputWindow = hwnd;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Windowed = !FULL_SCREEN;

	IDXGIDevice* pIDXGIDevice = NULL;
	IDXGIAdapter* pIDXGIAdapter = NULL;
	IDXGIFactory* pIDXGIFactory = NULL;
	hResult = device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pIDXGIDevice));
	if (hResult != S_OK)
	{
		SafeRelease(device_);
		SafeRelease(deviceContext_);
		SafeRelease(pIDXGIDevice);
		SafeRelease(pIDXGIAdapter);
		SafeRelease(pIDXGIFactory);
		throw PMD_FailCreateSwapChainException();
	}
	hResult = pIDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pIDXGIAdapter));
	if (hResult != S_OK)
	{
		SafeRelease(device_);
		SafeRelease(deviceContext_);
		SafeRelease(pIDXGIDevice);
		SafeRelease(pIDXGIAdapter);
		SafeRelease(pIDXGIFactory);
		throw PMD_FailCreateSwapChainException();
	}
	hResult = pIDXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pIDXGIFactory));
	if (hResult != S_OK)
	{
		SafeRelease(device_);
		SafeRelease(deviceContext_);
		SafeRelease(pIDXGIDevice);
		SafeRelease(pIDXGIAdapter);
		SafeRelease(pIDXGIFactory);
		throw PMD_FailCreateSwapChainException();
	}
	hResult = pIDXGIFactory->CreateSwapChain(device_, &swap_chain_desc, &swapChain_);
	if (hResult != S_OK)
	{
		SafeRelease(device_);
		SafeRelease(deviceContext_);
		SafeRelease(pIDXGIDevice);
		SafeRelease(pIDXGIAdapter);
		SafeRelease(pIDXGIFactory);
		throw PMD_FailCreateSwapChainException();
	}
	SafeRelease(pIDXGIDevice);
	SafeRelease(pIDXGIAdapter);
	SafeRelease(pIDXGIFactory);

	//Create RenderTargetView
	ID3D11Texture2D* backBuffer;
	hResult = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**> (&backBuffer)); //GetBuffer() will increase the COM reference count
	if (hResult != S_OK)
	{
		SafeRelease(swapChain_);
		SafeRelease(device_);
		SafeRelease(deviceContext_);
		throw PMD_FailCreateRenderTargetViewException();
	}

	hResult = device_->CreateRenderTargetView(backBuffer, NULL, &renderTargetView_);
	if (hResult != S_OK)
	{
		SafeRelease(backBuffer);
		SafeRelease(swapChain_);
		SafeRelease(device_);
		SafeRelease(deviceContext_);
		throw PMD_FailCreateRenderTargetViewException();
	}
	//Release
	SafeRelease(backBuffer); //Decrease the COM reference count

	//Create DepthStencilView
	D3D11_TEXTURE2D_DESC texture2d_desc;
	texture2d_desc.ArraySize = 1;							//Used for Texture Array. NOT useful here
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//Binding type
	texture2d_desc.CPUAccessFlags = 0;						//No access for CPU
	texture2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  //24bits for Depth, 8 bits for template
	texture2d_desc.Height = window_rect.bottom - window_rect.top;
	texture2d_desc.Width = window_rect.right - window_rect.left;
	texture2d_desc.MipLevels = 1;							//Not use Mipmap
	texture2d_desc.MiscFlags = 0;
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
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;				//For GPU Read & Write

	hResult = device_->CreateTexture2D(&texture2d_desc, NULL, &stencilBuffer_);
	if (hResult != S_OK)
	{
		SafeRelease(renderTargetView_);
		SafeRelease(swapChain_);
		SafeRelease(device_);
		SafeRelease(deviceContext_);
		throw PMD_FailCreateDepthStencilViewException();
	}

	hResult = device_->CreateDepthStencilView(stencilBuffer_, NULL, &depthStencilView_);
	if (hResult != S_OK)
	{
		SafeRelease(stencilBuffer_);
		SafeRelease(renderTargetView_);
		SafeRelease(swapChain_);
		SafeRelease(device_);
		SafeRelease(deviceContext_);
		throw PMD_FailCreateDepthStencilViewException();
	}

	//Bind to Rendering Pipeline
	deviceContext_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);

	//Set ViewPoint
	D3D11_VIEWPORT viewpoint;
	//#####WARNNING
	viewpoint.Width = static_cast<FLOAT>(window_rect.right - window_rect.left);
	viewpoint.Height = static_cast<FLOAT>(window_rect.bottom - window_rect.top);
	viewpoint.TopLeftX = 0.0f;
	viewpoint.TopLeftY = 0.0f;
	viewpoint.MinDepth = NEAR_POINT;
	viewpoint.MaxDepth = FAR_POINT;

	deviceContext_->RSSetViewports(1, &viewpoint);
}

D3D11Graphics::~D3D11Graphics()
{
	SafeRelease(device_);
	SafeRelease(deviceContext_);
	SafeRelease(swapChain_);
	SafeRelease(renderTargetView_);
	SafeRelease(stencilBuffer_);
	SafeRelease(depthStencilView_);
}

void D3D11Graphics::Render()
{
	DirectX::XMVECTORF32 color = { 0.f, 1.f, 0.f, 1.6f };
	deviceContext_->ClearRenderTargetView(renderTargetView_, reinterpret_cast<float*>(&color));
	deviceContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	swapChain_->Present(0, 0);
}

void D3D11Graphics::SafeRelease(IUnknown* pObject)
{
	if (pObject != NULL)
	{
		pObject->Release();
	}
}
