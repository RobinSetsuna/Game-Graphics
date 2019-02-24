#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Vertex.h"
#include "WICTextureLoader.h"

class Material
{
public:
	~Material();
	Material(SimpleVertexShader*, SimplePixelShader*, ID3D11ShaderResourceView*, ID3D11SamplerState*);
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	ID3D11SamplerState* GetSampler();
	ID3D11ShaderResourceView* GetSRV();
	void SetTexture();

private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* SRV;
	ID3D11SamplerState* sampler;

};