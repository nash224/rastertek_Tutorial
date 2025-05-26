//***************************************************************************************
// textclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_TEXTCLASS_H_
#define __DEFINE_TEXTCLASS_H_


//---------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include "textureclass.h"
#include "fontclass.h"


//---------------------------------------------------------------------------------------
// Geometry Shape struct 
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// Class name: TextClass
//---------------------------------------------------------------------------------------
/*
����:
	���� ������ ������ ��� �ִ� Ŭ����
	���� ������ ǥ���ϱ� ���� ���ؽ� ���� �� �ε��� ���۸� 
	���� �� ����, ������ ������ ������ ĸ��ȭ�մϴ�.

���:
	- ���ؽ� ���̴� �� �ȼ� ���̴� ���� �� ����
	- ������ ���� ���� ������ �׸��� ����� �Է� ���ձ� ����
	- ����� �ý����� ���� ���� ������ ���� ������ �ε�
*/
class TextClass
{
public:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

	TextClass();
	~TextClass();

	int GetIndexCount() const { return m_indexCount; }
	DirectX::XMFLOAT4 GetPixelColor() const { return m_pixelColor; }

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, 
		int maxLength, FontClass* Font, char* text, int positionX, int positionY, 
		float red, float green, float blue);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	bool UpdateText(ID3D11DeviceContext* deviceContext, FontClass* Font, 
		char* text, int positionX, int positionY, float red, float green, float blue);

private:

	/**
	 * @brief		InitializeBuffers		���� ������ ǥ���ϱ� ���� 
											���ؽ� ���� �� �ε��� ���� ����
	 *
	 * @param		device		D3D Device �ڵ�
	 *
	 * @return		bool		������ ���� ����
	 */
	bool InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FontClass* Font, 
		char* text, int positionX, int positionY, float red, float green, float blue);

	/**
	 * @brief		ShutdownBuffers			���� ������ �����ϱ� ���� �ڿ��� ��ȯ�Ѵ�.
	 *
	 * @return		void
	 */
	void ShutdownBuffers();

	/**
	 * @brief		RenderBuffers			���� ������ �ε��� ���� �� ���ؽ� ���� ����,
											���� ������ �׸��� ��� ����
	 *
	 * @param		deviceContext			�𵨿� ���� ���� ������ ������ ���������ο� ������ �ڵ�
	 *
	 * @return		void
	 */
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	int m_vertexCount;
	int m_indexCount;
	int m_screenWidth;
	int m_screenHeight;

	int m_maxLength;

	DirectX::XMFLOAT4 m_pixelColor;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
};


#endif // !__DEFINE_TEXTCLASS_H_
