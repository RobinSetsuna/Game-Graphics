#include "Material.h"

Material::~Material()
{
}

Material::Material(SimpleVertexShader * vs, SimplePixelShader * ps, ID3D11ShaderResourceView* srv, ID3D11SamplerState* sp)
{
	vertexShader = vs;
	pixelShader = ps;
	SRV = srv;
	sampler = sp;
}
void Material::SetTexture()
{
	pixelShader->SetShaderResourceView("diffuseTexture", SRV);
	pixelShader->SetSamplerState("basicSampler", sampler);
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}

ID3D11SamplerState * Material::GetSampler()
{
	return sampler;
}

ID3D11ShaderResourceView * Material::GetSRV()
{
	return SRV;
}


