#ifndef PMD_LIBRARY_H
#define PMD_LIBRARY_H

#include "DirectXMath.h"


#define PI 3.1415926f

namespace Colors
{
	DirectX::XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	DirectX::XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	DirectX::XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	DirectX::XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
}


#endif
