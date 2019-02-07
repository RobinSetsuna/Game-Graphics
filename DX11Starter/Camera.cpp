#include"Camera.h"


Camera::~Camera()
{

}

Camera::Camera(unsigned int width, unsigned int height)
{
	XMVECTOR pos = XMVectorSet(0, 0, -30, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMVECTOR ri = XMVectorSet(-1, 0, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
	
	XMStoreFloat3(&position, pos);
	XMStoreFloat3(&direction, dir);

	RotationAroundX = 0;
	RotationAroundY = 0;
}

void Camera::Update(XMVECTOR newPos, XMMATRIX V)
{
	XMStoreFloat3(&position, newPos);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
}

void Camera::SetRotationX(float _X)
{
	RotationAroundX = _X;
}

void Camera::SetRotationY(float _Y)
{
	RotationAroundY = _Y;
}

void Camera::SetPosition(float _X, float _Y, float _Z)
{
	position = XMFLOAT3(_X, _Y, _Z);
}

void Camera::SetRotation(float x, float y)
{
	RotationAroundX = x;
	RotationAroundY = y;
}

void Camera::Rotate()
{
	XMVECTOR Rotation = XMQuaternionRotationRollPitchYaw(RotationAroundX, RotationAroundY, 0);
	XMVECTOR camDirection = XMLoadFloat3(&direction);
	XMVECTOR newCamDirection = XMVector3Rotate(camDirection, Rotation);
	XMVECTOR camPosition = XMLoadFloat3(&position);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(camPosition, newCamDirection, up);

	XMStoreFloat3(&direction, newCamDirection);

	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

XMFLOAT3 Camera::GetPosition()
{
	return position;
}

XMFLOAT3 Camera::GetDirection()
{
	return direction;
}
