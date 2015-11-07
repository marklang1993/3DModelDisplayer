#include "DisplayForm.h"
#include "PrecureModelDisplayerException.h"

DisplayForm::DisplayForm(HINSTANCE hInstance, int nCmdShow, WNDPROC msgProcessor)
{
	//Set Handle of Instance
	this->hInstance_ = hInstance;
	this->nCmdShow_ = nCmdShow;
	this->msgProcessor_ = msgProcessor;
	//default size 800*600
	this->window_Width_ = 800;
	this->window_Height_ = 600;
	//Initialize
	newWindow_();
}

DisplayForm::DisplayForm(HINSTANCE hInstance, int nCmdShow, WNDPROC msgProcessor, int window_Width, int window_Height)
{
	//Set Handle of Instance
	this->hInstance_ = hInstance;
	this->nCmdShow_ = nCmdShow;
	this->msgProcessor_ = msgProcessor;
	//Set Window Size
	this->window_Width_ = window_Width;
	this->window_Height_ = window_Height;
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
	MSG msg;
	//Message Loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HWND DisplayForm::get_HWND()
{
	return this->hWindow_;
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
	wndWindow_.lpfnWndProc = msgProcessor_;
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
