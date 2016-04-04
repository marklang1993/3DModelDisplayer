#include "DisplayForm.h"
#include "PrecureModelDisplayerException.h"


//Global Message Process Function
DisplayForm* MainForm = NULL;
LRESULT CALLBACK MsgProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	return MainForm->WndProc(hwnd, message, wparam, lparam);
}


DisplayForm::DisplayForm(HINSTANCE hInstance, int nCmdShow, int window_Width, int window_Height)
{
	//Initialize Global Message Process Function
	MainForm = this;
	//Set Pointer to D3DX11 Graphics
	pGraphics_ = NULL;
	//Set Handle of Instance
	hInstance_ = hInstance;
	nCmdShow_ = nCmdShow;
	//Set Window Size
	window_Width_ = window_Width;
	window_Height_ = window_Height;
	//Initialize
	newWindow_();
}

DisplayForm::~DisplayForm()
{
	//For Window
	DestroyWindow(hWindow_);
	hWindow_ = NULL;
	UnregisterClass(L"PrecureModelDisplayer", hInstance_);
}

void DisplayForm::startMsgLoop()
{
	MSG msg = {0};
	//Message Loop
	while (msg.message != WM_QUIT)
	{
		//There are some messages needed to be processed
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Do Drawing
			pGraphics_->Render();
			
		}
	}
}

LRESULT DisplayForm::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY: // Close
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN: // Mouse: Left Button Down
		
		break;
	case WM_RBUTTONDOWN: // Mouse: Right Button Down
		PostQuitMessage(0);
		break;
	case WM_MOUSEWHEEL: // Mouse: Wheel Rotate
		GET_WHEEL_DELTA_WPARAM(wparam);
		break;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}

HWND DisplayForm::getHWND()
{
	return this->hWindow_;
}

void DisplayForm::setGraphics(D3D11Graphics* pGraphics)
{
	pGraphics_ = pGraphics;
}

void DisplayForm::newWindow_()
{
	//Get Windows screen size
	screenWidth_ = GetSystemMetrics(SM_CXSCREEN);
	screenHeight_ = GetSystemMetrics(SM_CYSCREEN);
	//Define mainwindow class
	wndWindow_.cbClsExtra = 0;
	wndWindow_.cbWndExtra = 0;
	wndWindow_.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wndWindow_.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndWindow_.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndWindow_.hInstance = hInstance_;
	wndWindow_.lpfnWndProc = MsgProc;
	wndWindow_.lpszClassName = L"PrecureModelDisplayer";
	wndWindow_.lpszMenuName = NULL;
	wndWindow_.style = CS_HREDRAW | CS_VREDRAW;
	//Register mainwindow class
	if (!RegisterClass(&wndWindow_))
	{
		throw PMD_FailCreateWindowException();
	}
	//Create Window
	hWindow_ = CreateWindow(L"PrecureModelDisplayer", L"Precure Model Displayer", WS_OVERLAPPEDWINDOW,
		(screenWidth_ - window_Width_) / 2, (screenHeight_ - window_Height_) / 2,
		window_Width_, window_Height_,
		NULL, NULL, hInstance_, NULL);
	//Show AND Update Window
	ShowWindow(hWindow_, nCmdShow_);
	UpdateWindow(hWindow_);
}
