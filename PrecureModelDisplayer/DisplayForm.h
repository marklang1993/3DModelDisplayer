#ifndef DISPLAYFORM_H
#define DISPLAYFORM_H

#include "D3D11Graphics.h"
#include "GameTimer.h"

#include "Windows.h"

class DisplayForm
{

public:
	DisplayForm(HINSTANCE hInstance, int nCmdShow, int window_Width, int window_Height);	// Constructor: Given size
	~DisplayForm();																			// Destructor

	void startMsgLoop();																	// Start Message Loop
	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);		// Windows Message Processing Function

	HWND getHWND();																			// Accessor - Handle of Window
	void setGraphics(D3D11Graphics* pGraphics);												// Mutator - Pointer to D3DX11 Graphics

private:
	//Windows API parameters
	HINSTANCE hInstance_;
	int nCmdShow_;
	HWND hWindow_;
	WNDCLASS wndWindow_;
	//Screen & Window size
	int screenWidth_;
	int screenHeight_;
	int window_Width_;
	int window_Height_;
	//D3DX11 Graphics
	D3D11Graphics* pGraphics_;
	//Timer
	GameTimer* timer_;
	double waitInterval_;		// Waiting time of updating FPS
	//Mouse Move
	POINT initMousPos_;

	//Helper Functions
	void newWindow_();			// Initialize Window

	//Event Handler
	void OnMouseUp(WPARAM button, int x, int y);
	void OnMouseDown(WPARAM button, int x, int y);
	void OnMouseMove(WPARAM button, int x, int y);
	void OnMouseWheel(int val);
};



#endif
