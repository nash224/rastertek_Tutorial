//***************************************************************************************
// cameraclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "cameraclass.h"

using namespace DirectX;

CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_viewMatrix = XMMatrixIdentity();
}

CameraClass::~CameraClass()
{

}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

DirectX::XMFLOAT3 CameraClass::GetPosition() const
{
	return DirectX::XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

DirectX::XMFLOAT3 CameraClass::GetRotation() const
{
	return DirectX::XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// ������ up ����
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	// ī�޶� ��ġ
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	positionVector = XMLoadFloat3(&position);

	// ���� ����
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// ȸ�� �� 
	pitch = DirectX::XMConvertToRadians(m_rotationX);
	yaw = DirectX::XMConvertToRadians(m_rotationY);
	roll = DirectX::XMConvertToRadians(m_rotationZ);

	// ���� ������� ������ ȸ�� ���
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// ī�޶� �ٶ󺸴� �������� ��ȯ
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);

	// ī�޶��� ���� ����
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// ī�޶��� �ٶ󺸴� �������� ��ȯ
	lookAtVector = XMVectorAdd(lookAtVector, positionVector);

	// ī�޶��� ��ġ, ���� �׸��� ���� ���ͷ� �� ��� ����
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}
