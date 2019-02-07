#include "Material.h"

Material::~Material()
{
}

Material::Material(SimpleVertexShader * vs, SimplePixelShader * ps)
{
	vertexShader = vs;
	pixelShader = ps;
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}
