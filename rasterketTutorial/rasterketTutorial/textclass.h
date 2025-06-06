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
목적:
	기하 도형의 정보를 담고 있는 클래스
	기하 도형을 표현하기 위해 버텍스 버퍼 및 인덱스 버퍼를 
	생성 및 관리, 해제를 포함한 과정을 캡슐화합니다.

기능:
	- 버텍스 쉐이더 및 픽셀 쉐이더 생성 및 관리
	- 렌더링 전에 기하 도형을 그리는 방법과 입력 조합기 세팅
	- 입출력 시스템을 통해 기하 도형의 정점 데이터 로드
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
	 * @brief		InitializeBuffers		기하 도형을 표현하기 위한 
											버텍스 버퍼 및 인덱스 버퍼 생성
	 *
	 * @param		device		D3D Device 핸들
	 *
	 * @return		bool		버퍼의 생성 유무
	 */
	bool InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FontClass* Font, 
		char* text, int positionX, int positionY, float red, float green, float blue);

	/**
	 * @brief		ShutdownBuffers			기하 도형을 구성하기 위한 자원을 반환한다.
	 *
	 * @return		void
	 */
	void ShutdownBuffers();

	/**
	 * @brief		RenderBuffers			기하 도형의 인덱스 버퍼 및 버텍스 버퍼 세팅,
											기하 도형을 그리는 방법 세팅
	 *
	 * @param		deviceContext			모델에 대한 기하 도형을 렌더링 파이프라인에 부착할 핸들
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
