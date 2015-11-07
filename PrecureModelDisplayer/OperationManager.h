#ifndef OPERATIONMANAGER_H
#define OPERATIONMANAGER_H

#include "Windows.h"

#include "D3D11Graphics.h"

//Static Class

class OpeartionManager
{
public:
	OpeartionManager() = delete;

	static void set_pGraphics(D3D11Graphics* pGraphics_);

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		// Message Processor
private:
	static D3D11Graphics* pGraphics_;
};


#endif