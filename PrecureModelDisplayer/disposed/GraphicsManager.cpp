#include "GraphicsManager.h"

GraphicsManager::GraphicsManager(HWND hwnd)
{
	graphics_ = NULL;
	graphics_ = new D3D11Graphics(hwnd);
}

GraphicsManager::~GraphicsManager()
{
	if (graphics_ != NULL)
	{
		delete graphics_;
		graphics_ = NULL;
	}
}