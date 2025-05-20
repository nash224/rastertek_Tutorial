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
	- 직사광, 환경광에 대한 데이터 제공
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

	void SetAmbientColor(float red, float green, float blue, float alpha) 
	{
		m_ambientColor = DirectX::XMFLOAT4{ red, green, blue, alpha };
	}

	DirectX::XMFLOAT4 GetAmbientColor() const { return m_ambientColor; }

	void SetDiffuseColor(float red, float green, float blue, float alpha) 
	{
		m_diffuseColor = DirectX::XMFLOAT4{ red, green, blue, alpha };
	}

	DirectX::XMFLOAT4 GetDiffuseColor() const { return m_diffuseColor; }

	void SetDirection(DirectX::XMFLOAT3 _direction) { m_direction = _direction; }
	void SetDirection(float x, float y, float z)
	{
		m_direction = DirectX::XMFLOAT3{ x,y,z };
	}

	DirectX::XMFLOAT3 GetDirection() const { return m_direction; }

	void SetSpecularColor(float red, float green, float blue, float alpha)
	{
		m_specularColor = DirectX::XMFLOAT4{ red, green, blue, alpha };
	}

	DirectX::XMFLOAT4 GetSpecularColor() const { return m_specularColor; }

	void SetSpecularColor(float specularPower)
	{
		m_specularPower = specularPower;
	}

	float GetSpecularPower() const { return m_specularPower; }


private:
	DirectX::XMFLOAT4 m_ambientColor;
	DirectX::XMFLOAT4 m_diffuseColor;
	DirectX::XMFLOAT3 m_direction;
	DirectX::XMFLOAT4 m_specularColor;
	float m_specularPower;

};


#endif // !__DEFINE_LIGHTCLASS_H_
