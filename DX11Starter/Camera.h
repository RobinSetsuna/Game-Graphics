#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Vertex.h"
using namespace DirectX;

class Camera
{
public:
	~Camera();
	Camera(unsigned int, unsigned int);
	void Update(XMVECTOR, XMMATRIX);
	void SetRotationX(float);
	void SetRotationY(float);
	void SetPosition(float, float, float);
	void SetRotation(float x, float y);
	void Rotate();
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetDirection();

private:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	XMFLOAT3 position;
	XMFLOAT3 direction;

	float RotationAroundX;
	float RotationAroundY;
};

