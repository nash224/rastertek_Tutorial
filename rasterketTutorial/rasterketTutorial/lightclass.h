//***************************************************************************************
// lightclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_LIGHTCLASS_H_
#define __DEFINE_LIGHTCLASS_H_


//---------------------------------------------------------------------------------------
// INCLUDES 
//---------------------------------------------------------------------------------------
#include <DirectXMath.h>

//---------------------------------------------------------------------------------------
// Class name: LightClass
//---------------------------------------------------------------------------------------
/*
목적:
	렌더링에 필요한 빛의 정보를 전달하기 위한 클래스

기능:
	- 라이팅 정보 Setter 및 Getter 인터페이스 제공
*/
class LightClass
{
public:
	// constructor & destructor
	LightClass();
	~LightClass();

	// delete function
	LightClass(const LightClass& _Other) = delete;
	LightClass(LightClass&& _Other) noexcept = delete;
	LightClass& operator=(const LightClass& _Other) = delete;
	LightClass& operator=(LightClass&& _Other) noexcept = delete;

	void SetDiffuseColor(float red, float green, float blue, float alpha) 
	{
		m_diffuseColor = DirectX::XMFLOAT4{ red, green, blue, alpha };
	}

	DirectX::XMFLOAT4 GetDiffuseColor() const { return m_diffuseColor; }

	void SetDirection(float x, float y, float z)
	{
		m_direction = DirectX::XMFLOAT3{ x,y,z };
	}

	DirectX::XMFLOAT3 GetDirection() const { return m_direction; }


private:
	DirectX::XMFLOAT4 m_diffuseColor;
	DirectX::XMFLOAT3 m_direction;

};


#endif // !__DEFINE_LIGHTCLASS_H_
