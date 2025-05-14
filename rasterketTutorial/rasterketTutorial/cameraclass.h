//***************************************************************************************
// cameraclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_CAMERACLASS_H_
#define __DEFINE_CAMERACLASS_H_


//---------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------
#include <directxmath.h>


/*
����:
	ī�޶� ǥ���ϱ� ���� Ŭ����
	ī�޶� ���� ��ġ, ȸ�� ������ �������� ����� �� ����� 
	�����մϴ�.

���:
	- ī�޶� ���ۿ� ���õ� ���
	- �� �����Ӹ��� �� ��� �ʱ�ȭ
*/

class CameraClass
{
public:
	// constructor & destructor
	CameraClass();
	~CameraClass();

	// delete function
	CameraClass(const CameraClass& _Other) = delete;
	CameraClass(CameraClass&& _Other) noexcept = delete;
	CameraClass& operator=(const CameraClass& _Other) = delete;
	CameraClass& operator=(CameraClass&& _Other) noexcept = delete;

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	DirectX::XMFLOAT3 GetPosition() const;
	DirectX::XMFLOAT3 GetRotation() const;


	/**
	 * @brief		Render		����� ������Ʈ
	 *
	 * @return		void
	 */
	void Render();

	/**
	 * @brief		GetViewMatrix		ī�޶��� view ��� ��ȯ
	 *
	 * @param		_viewMatrix			�� ���
	 * 
	 * @return		void
	 *
	 * @warning		CameraClass::Render���� viewMatrix�� ������Ʈ�մϴ�.
	 *				���� �ش� �Լ��� ȣ��ǰ� ����ؾ� �մϴ�.
	 */
	void GetViewMatrix(DirectX::XMMATRIX& _viewMatrix)const { _viewMatrix = m_viewMatrix; }

protected:

private:
	float m_positionX;
	float m_positionY;
	float m_positionZ;

	float m_rotationX;
	float m_rotationY;
	float m_rotationZ;

	DirectX::XMMATRIX m_viewMatrix;

};


#endif // !__DEFINE_CAMERACLASS_H_
