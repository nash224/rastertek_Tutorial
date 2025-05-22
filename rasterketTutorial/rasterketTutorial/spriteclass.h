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
����:
	 2D ȭ�鿡 ǥ�õǴ� ��������Ʈ�� ��Ÿ���� Ŭ����

���:
	- �ؽ�ó �Ҵ� �� ������
	- �ִϸ��̼� ������ ����
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
	 * @brief		Update		�ִϸ��̼� ������ ��ȯ ����
	 *
	 * @param		frameTime		���� �����Ӻ��� ���� �����ӱ��� �ɸ� �ð�
	 *
	 * @return		UpdateBuffers���� ������ ũ�⸦ �����ϱ� ������,
	 *				UpdateBuffers�� ȣ��Ǳ� ���� ȣ��Ǿ�� �մϴ�.
	 */
	void Update(float frameTime);

	/**
	 * @brief		UpdateBuffers		���� ���� ������ ����
	 *
	 * @param		deviceContext		������ �����͸� �����ϱ� ���� ���������� �������̽�
	 *
	 * @return		bool				�������� �̹����� ũ�⳪ ��ġ�� ������ ���
	 *
	 * @warning		��ũ���� ��� �̹����� ũ�� �Ǵ� ��ġ�� �������� ���� ���,
	 *				������Ʈ�� �������� ����
	 */
	bool UpdateBuffers(ID3D11DeviceContext* deviceContext);

private:

	/**
	 * @brief		InitializeBuffers		���� ������ ǥ���ϱ� ���� 
											���ؽ� ���� �� �ε��� ���� ����
	 *
	 * @param		device		D3D Device �ڵ�
	 *
	 * @return		bool		������ ���� ����
	 */
	bool InitializeBuffers(ID3D11Device* device);

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

	/**
	 * @brief		LoadTexture		�ؽ�ó �ε� �� SRV ��ü ����
	 *
	 * @return		bool		SRV ��ü ���� ����
	 */
	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);

	/**
	 * @brief		ReleaseTexture		�ؽ�ó �ڿ� ��ȯ
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
