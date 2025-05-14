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

	// 월드의 up 벡터
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	upVector = XMLoadFloat3(&up);

	// 카메라 위치
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	positionVector = XMLoadFloat3(&position);

	// 정면 벡터
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// 회전 축 
	pitch = DirectX::XMConvertToRadians(m_rotationX);
	yaw = DirectX::XMConvertToRadians(m_rotationY);
	roll = DirectX::XMConvertToRadians(m_rotationZ);

	// 축을 기반으로 생성한 회전 행렬
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// 카메라가 바라보는 방향으로 변환
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);

	// 카메라의 수직 벡터
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// 카메라의 바라보는 시점으로 변환
	lookAtVector = XMVectorAdd(lookAtVector, positionVector);

	// 카메라의 위치, 시점 그리고 수직 벡터로 뷰 행렬 생성
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}
