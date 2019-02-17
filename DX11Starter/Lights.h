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

	void Init(XMFLOAT4 ac, XMFLOAT4 dc, XMFLOAT3 dir)
	{
		AmbientColor = ac;
		DiffuseColor = dc;
		Direction = dir;
	}
};