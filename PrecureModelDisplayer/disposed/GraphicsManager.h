#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include "Windows.h"

#include "D3D11Graphics.h"



class GraphicsManager
{
public:
	GraphicsManager(HWND hwnd);
	~GraphicsManager();

private:
	D3D11Graphics* graphics_;
};


#endif
