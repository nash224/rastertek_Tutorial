//***************************************************************************************
// spriteclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_SPRITECLASS_H_
#define __DEFINE_SPRITECLASS_H_


//---------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include "textureclass.h"


//---------------------------------------------------------------------------------------
// Geometry Shape struct 
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// Class name: spriteclass
//---------------------------------------------------------------------------------------
/*
목적:
	 2D 화면에 표시되는 스프라이트를 나타내는 클래스

기능:
	- 텍스처 할당 및 렌더링
	- 애니메이션 프레임 제어
*/
class SpriteClass
{
public:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

	SpriteClass();
	~SpriteClass();

	int GetIndexCount() const { return m_indexCount; }
	ID3D11ShaderResourceView* GetTexture() const { return m_textures[m_currentTexture].GetTexture(); }
	void SetRenderLocation(int x, int y) { m_renderX = x; m_renderY = y; }
	void SetRenderScale(float x, float y) { m_renderScaleX = x; m_renderScaleY = y; }
	void SetDuration(float duration) { m_cycleTime = duration; };

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, 
		int screenWidth, int screenHeight, char* spriteFilename, int renderX, int renderY, float ImageScaleX, float ImageScaleY);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext);


	/**
	 * @brief		Update		애니메이션 프레임 전환 제어
	 *
	 * @param		frameTime		이전 프레임부터 현재 프레임까지 걸린 시간
	 *
	 * @return		UpdateBuffers에서 도형의 크기를 결정하기 때문에,
	 *				UpdateBuffers가 호출되기 전에 호출되어야 합니다.
	 */
	void Update(float frameTime);

	/**
	 * @brief		UpdateBuffers		정점 버퍼 데이터 변경
	 *
	 * @param		deviceContext		버퍼의 데이터를 수정하기 위한 파이프라인 인터페이스
	 *
	 * @return		bool				렌더링될 이미지의 크기나 위치가 동일할 경우
	 *
	 * @warning		스크린에 띄울 이미지의 크기 또는 위치가 변경하지 않을 경우,
	 *				업데이트를 수행하지 않음
	 */
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext);

private:

	/**
	 * @brief		InitializeBuffers		기하 도형을 표현하기 위한 
											버텍스 버퍼 및 인덱스 버퍼 생성
	 *
	 * @param		device		D3D Device 핸들
	 *
	 * @return		bool		버퍼의 생성 유무
	 */
	bool InitializeBuffers(ID3D11Device* device);

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

	/**
	 * @brief		LoadTexture		텍스처 로딩 및 SRV 객체 생성
	 *
	 * @return		bool		SRV 객체 생성 유무
	 */
	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);

	/**
	 * @brief		ReleaseTexture		텍스처 자원 반환
	 *
	 * @return		void
	 */
	void ReleaseTexture();

private:
	int m_vertexCount;
	int m_indexCount;

	int m_screenWidth;
	int m_screenHeight;
	int m_bitmapWidth;
	int m_bitmapHeight;
	float m_renderScaleX;
	float m_renderScaleY;
	float m_prevScaleX;
	float m_prevScaleY;
	int m_renderX;
	int m_renderY;
	int m_prevPosX;
	int m_prevPosY;


	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	TextureClass* m_textures;
	int m_textureCount;
	float m_cycleTime;
	float m_frameTime;
	int m_currentTexture;
};


#endif // !__DEFINE_SPRITECLASS_H_
