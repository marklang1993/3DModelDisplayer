#ifndef DISPLAYFORM_H
#define DISPLAYFORM_H

#include "Windows.h"

class DisplayForm
{
public:
	DisplayForm(HINSTANCE hInstance, int nCmdShow, WNDPROC msgProcessor); // Constructor: (default) 800*600
	DisplayForm(HINSTANCE hInstance, int nCmdShow, WNDPROC msgProcessor, int window_Width, int window_Height); // Constructor: Given size
	~DisplayForm();			// Destructor

	void startMsgLoop();	// Start Message Loop

	HWND get_HWND();		// Accessor - Handle of Window

private:
	//Windows API parameters
	HINSTANCE hInstance_;
	int nCmdShow_;
	HWND hWindow_;
	WNDCLASS wndWindow_;
	WNDPROC msgProcessor_;
	//Screen & Window size
	int screenWidth_;
	int screenHeight_;
	int window_Width_;
	int window_Height_;

	void newWindow_();		// Helper Function - Initialize Window
};



#endif
