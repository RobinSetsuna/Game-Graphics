  #pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "Material.h"
#include <DirectXMath.h>
#include "Vertex.h"
using namespace DirectX;

class Entity
{
public:
	Entity(Mesh*, Material*);
	Entity();
	//~Entity();
	Mesh * mesh;

	XMFLOAT4X4 GetWorldMatrix();
	XMFLOAT3 GetTranslation();
	XMFLOAT3 GetScale();
	XMFLOAT3 GetRotation();

	void SetWorldMatrix(XMFLOAT4X4);

	void SetTranslation(XMFLOAT3);
	void SetScale(XMFLOAT3);
	void SetRoation(XMFLOAT3);

	void SetTranslation(float, float, float);
	void SetScale(float, float, float);
	void SetRotation(float, float, float);

	void Move(float, float, float);
	void Scale(float, float, float);
	void Rotate(float, float, float);

	void PrepareMaterial(XMFLOAT4X4, XMFLOAT4X4);
	void CopyBufferToShader();
	Material* GetMaterial();

private:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 translation;
	Material* mat;
	bool update = true;
	void UpdateWorldMatrix();
};