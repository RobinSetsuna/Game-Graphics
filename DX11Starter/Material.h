#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Vertex.h"

class Material
{
public:
	~Material();
	Material(SimpleVertexShader*, SimplePixelShader*);
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();


private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;


};