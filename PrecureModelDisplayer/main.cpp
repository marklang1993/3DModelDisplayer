#include "DisplayForm.h"
#include "D3D11Graphics.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, PSTR lpszCmdLine, int nCmdShow)
{
	//Initialize Main Form
	DisplayForm* displayform = new DisplayForm(hInstance, nCmdShow, 800, 600);
	//Initialize Graphics
	D3D11Graphics* graphics = new D3D11Graphics(displayform->getHWND());
	//Bind Graphics to Main Form
	displayform->setGraphics(graphics);
	//Run
	displayform->startMsgLoop();
	

	//Release Resources
	delete graphics;
	delete displayform;
	hInstance = NULL;

	return 0;
}
