#ifndef PMD__LIBRARY_H
#define PMD__LIBRARY_H

#include "DirectXMath.h"


#define PI 3.1415926f

namespace Colors
{
	static DirectX::XMVECTORF32 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	static DirectX::XMVECTORF32 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
	static DirectX::XMVECTORF32 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	static DirectX::XMVECTORF32 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
	static DirectX::XMVECTORF32 White = { 1.0f, 1.0f, 1.0f, 1.0f };
	static DirectX::XMVECTORF32 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
	static DirectX::XMVECTORF32 Grey = { 0.753f, 0.753f, 0.753f, 1.0f };
}


struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};

#endif
