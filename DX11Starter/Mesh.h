#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Vertex.h"
using namespace DirectX;

class Mesh
{
public:
	~Mesh();
	void CreateBasicGeometry(Vertex*, int, UINT*, int, ID3D11Device*);
	Mesh::Mesh(const char * objFile, ID3D11Device* d);
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexNumber();

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Device* device;

	int vertexArrayNumber;
	int indexArrayNumber;

	Vertex* vertices;
	UINT* indices;


};