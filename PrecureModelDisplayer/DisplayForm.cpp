#include "DisplayForm.h"
#include "PrecureModelDisplayerException.h"

#include <sstream>
#include <windowsx.h>

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
	//Initialize Timer
	timer_ = new GameTimer();
	waitInterval_ = 0.0;
	timer_->Start();

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
	//Others
	delete timer_;
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
			//Calculate FPS
			timer_->Tick();
			if (waitInterval_ > 500.0)
			{
				//Update FPS
				double currentInterval = timer_->getDeltaTime();
				double FPS = 1000.0 / currentInterval;
				//Preparing Text
				std::wostringstream FPS_woss;
				FPS_woss.precision(6);
				FPS_woss << L"FPS: ";
				FPS_woss << FPS;
				//Display
				SetWindowText(hWindow_, FPS_woss.str().c_str());
				//Reset
				waitInterval_ = 0.0;
			}
			else
			{
				//Wait
				waitInterval_ += timer_->getDeltaTime();
			}

		}
	}
}

LRESULT DisplayForm::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:		// Close
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:		// Mouse: Move
		OnMouseMove(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		break;

	case WM_LBUTTONUP:		// Mouse: Left Button Up
		OnMouseUp(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		break;

	case WM_LBUTTONDOWN:	// Mouse: Left Button Down
		OnMouseDown(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		break;

	case WM_RBUTTONDOWN:	// Mouse: Right Button Down
		PostQuitMessage(0);
		break;

	case WM_MOUSEWHEEL:		// Mouse: Wheel Rotate
		OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wparam));
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

void DisplayForm::OnMouseUp(WPARAM button, int x, int y)
{
	ReleaseCapture();
}

void DisplayForm::OnMouseDown(WPARAM button, int x, int y)
{
	initMousPos_.x = x;
	initMousPos_.y = y;
	SetCapture(hWindow_);
}

void DisplayForm::OnMouseMove(WPARAM button, int x, int y)
{
	if (button & MK_LBUTTON)
	{
		//Mouse Left Button Down
		POINT deltaMousePos;
		deltaMousePos.x = x - initMousPos_.x;
		deltaMousePos.y = -(y - initMousPos_.y);
		//Change Camera Position
		float rCamera;
		int alphaCamera;
		int betaCamera;
		pGraphics_->getCameraPos(rCamera, alphaCamera, betaCamera);
		//Rollback
		alphaCamera = (alphaCamera + (int)(deltaMousePos.x / 60.0)) % 360;
		betaCamera = (betaCamera + (int)(deltaMousePos.y / 60.0)) % 360;
		pGraphics_->setCameraPos(rCamera, alphaCamera, betaCamera);
	}
}

void DisplayForm::OnMouseWheel(int val)
{
	//Get Current Camera Position
	float rCamera;
	int alphaCamera;
	int betaCamera;
	pGraphics_->getCameraPos(rCamera, alphaCamera, betaCamera);
	//Check Bound
	if (val > 0)		//Up: Zoom In
	{
		//Change Camera Position
		if (rCamera > 0.1f)
		{
			rCamera -= 0.1f;
		}
	}
	else if (val < 0)	//Up: Zoom Out
	{
		//Change Camera Position
		if (rCamera < 100.0f)
		{
			rCamera += 0.1f;
		}
	}
	//Set New Camera Position
	pGraphics_->setCameraPos(rCamera, alphaCamera, betaCamera);
}