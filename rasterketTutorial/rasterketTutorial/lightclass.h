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
����:
	�������� �ʿ��� ���� ������ �����ϱ� ���� Ŭ����

���:
	- ���籤, ȯ�汤�� ���� ������ ����
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

	void SetDirection(float x, float y, float z)
	{
		m_direction = DirectX::XMFLOAT3{ x,y,z };
	}

	DirectX::XMFLOAT3 GetDirection() const { return m_direction; }


private:
	DirectX::XMFLOAT4 m_ambientColor;
	DirectX::XMFLOAT4 m_diffuseColor;
	DirectX::XMFLOAT3 m_direction;

};


#endif // !__DEFINE_LIGHTCLASS_H_
