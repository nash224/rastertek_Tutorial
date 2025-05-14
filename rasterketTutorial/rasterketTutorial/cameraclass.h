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
목적:
	카메라를 표현하기 위한 클래스
	카메라에 대한 위치, 회전 정보와 렌더링에 사용할 뷰 행렬을 
	생성합니다.

기능:
	- 카메라 조작에 관련된 기능
	- 매 프레임마다 뷰 행렬 초기화
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
	 * @brief		Render		뷰행렬 업데이트
	 *
	 * @return		void
	 */
	void Render();

	/**
	 * @brief		GetViewMatrix		카메라의 view 행렬 반환
	 *
	 * @param		_viewMatrix			뷰 행렬
	 * 
	 * @return		void
	 *
	 * @warning		CameraClass::Render에서 viewMatrix를 업데이트합니다.
	 *				따라서 해당 함수가 호출되고 사용해야 합니다.
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
