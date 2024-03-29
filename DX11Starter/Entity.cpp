#include "Entity.h"

Entity::Entity(Mesh* _mesh, Material* _mat)
{
	mesh = _mesh;
	mat = _mat;

	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));

	translation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(0.5, 0.5, 0.5);
	rotation = XMFLOAT3(0, 0, 0);
}

Entity::Entity()
{
}

XMFLOAT4X4 Entity::GetWorldMatrix()
{
	if (update == true)
	{
		UpdateWorldMatrix();
	}
	return worldMatrix;
}

void Entity::SetWorldMatrix(XMFLOAT4X4 _worldMatrix)
{
	worldMatrix = _worldMatrix;
	update = true;
}

XMFLOAT3 Entity::GetTranslation()
{
	return translation;
}

void Entity::SetTranslation(XMFLOAT3 _translation)
{
	translation = _translation;
	update = true;
}

XMFLOAT3 Entity::GetScale()
{
	return scale;
}

void Entity::SetScale(XMFLOAT3 _scale)
{
	scale = _scale;
	update = true;
}

XMFLOAT3 Entity::GetRotation()
{
	return rotation;
}

void Entity::SetRoation(XMFLOAT3 _rotation)
{
	rotation = _rotation;
	update = true;
}

void Entity::SetTranslation(float x, float y, float z)
{
	translation = XMFLOAT3(x,y,z);
	update = true;
}

void Entity::SetRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
	update = true;
}

void Entity::SetScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
	update = true;
}
 
void Entity::Move(float x, float y, float z)
{
	XMVECTOR incremental = XMLoadFloat3(&XMFLOAT3(x, y, z));
	XMVECTOR trans = XMLoadFloat3(&GetTranslation());
	XMVECTOR new_vec = XMVectorAdd(trans, incremental); 
	XMFLOAT3 new_trans;
	XMStoreFloat3(&new_trans, new_vec);
	SetTranslation(new_trans);
}

void Entity::Scale(float x, float y, float z)
{
	XMVECTOR incremental = XMLoadFloat3(&XMFLOAT3(x, y, z));
	XMVECTOR scale = XMLoadFloat3(&GetScale());
	XMVECTOR new_vec = XMVectorAdd(scale, incremental);
	XMFLOAT3 new_trans;
	XMStoreFloat3(&new_trans, new_vec);
	SetScale(new_trans);
}

void Entity::Rotate(float x, float y, float z)
{
	XMVECTOR incremental = XMLoadFloat3(&XMFLOAT3(x, y, z));
	XMVECTOR rotation = XMLoadFloat3(&GetRotation());
	XMVECTOR new_vec = XMVectorAdd(rotation, incremental);
	XMFLOAT3 new_trans;
	XMStoreFloat3(&new_trans, new_vec);
	SetRoation(new_trans);
}

void Entity::PrepareMaterial(XMFLOAT4X4 v, XMFLOAT4X4 p)
{
	mat->GetVertexShader()->SetMatrix4x4("world", GetWorldMatrix());
	mat->GetVertexShader()->SetMatrix4x4("view", v);
	mat->GetVertexShader()->SetMatrix4x4("projection", p);
}

Material * Entity::GetMaterial()
{
	return mat;
}

void Entity::UpdateWorldMatrix()
{
	XMMATRIX trans = XMMatrixTranslation(translation.x, translation.y, translation.z);

	XMMATRIX rot_x = XMMatrixRotationX(rotation.x);
	XMMATRIX rot_y = XMMatrixRotationY(rotation.y);
	XMMATRIX rot_z = XMMatrixRotationZ(rotation.z);

	XMMATRIX scal = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX world =  scal * rot_z * rot_y * rot_x * trans;

	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));

	update = false;
}

void Entity::CopyBufferToShader()
{
	mat->GetVertexShader()->CopyAllBufferData();
	mat->GetVertexShader()->SetShader();
	mat->GetPixelShader()->CopyAllBufferData();
	mat->GetPixelShader()->SetShader();
}