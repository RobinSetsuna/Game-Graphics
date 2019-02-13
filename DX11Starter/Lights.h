#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Vertex.h"
using namespace DirectX;

struct DiretionalLight
{
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;

	void Init()
	{
		AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		DiffuseColor = XMFLOAT4(0, 0, 1, 1);
		Direction = XMFLOAT3(1, -1, 0);
	}
};