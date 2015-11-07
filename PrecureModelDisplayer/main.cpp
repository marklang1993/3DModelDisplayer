#include "DisplayForm.h"
#include "OperationManager.h"
#include "D3D11Graphics.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, PSTR lpszCmdLine, int nCmdShow)
{
	//Initialize Main Form
	DisplayForm* displayform = new DisplayForm(hInstance, nCmdShow, OpeartionManager::WndProc);
	//Initialize Graphics
	D3D11Graphics* graphics = new D3D11Graphics(displayform->get_HWND());
	//Bind Graphics to Operation Manager
	OpeartionManager::set_pGraphics(graphics);
	//Run
	displayform->startMsgLoop();
	

	//Release Resources
	delete graphics;
	delete displayform;
	hInstance = NULL;

	return 0;
}
