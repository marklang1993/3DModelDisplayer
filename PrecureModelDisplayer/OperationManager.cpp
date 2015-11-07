#include "OperationManager.h"

void OpeartionManager::set_pGraphics(D3D11Graphics* pGraphics_)
{
	OpeartionManager::pGraphics_ = pGraphics_;
}

LRESULT CALLBACK OpeartionManager::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY: // Close
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN: // Mouse: Left Button Down
		pGraphics_->Render();
		break;
	case WM_RBUTTONDOWN: // Mouse: Right Button Down
		PostQuitMessage(0);
		break;
	case WM_MOUSEWHEEL: // Mouse: Wheel Rotate
		GET_WHEEL_DELTA_WPARAM(wparam);
		break;
	default:
		return (DefWindowProc(hwnd, message, wparam, lparam));
	}
	return 0;
}


D3D11Graphics* OpeartionManager::pGraphics_ = NULL;