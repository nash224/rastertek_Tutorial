//***************************************************************************************
// bitmapclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_BITMAPCLASS_H_
#define __DEFINE_BITMAPCLASS_H_


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
// Class name: BitmapClass
//---------------------------------------------------------------------------------------
/*
����:
	��ũ�� ȭ�鿡 2D �̹����� ����ϴ� Ŭ����
	���� ������ ǥ���ϱ� ���� ���ؽ� ���� �� �ε��� ���۸� 
	���� �� ����, ������ ������ ������ ĸ��ȭ�մϴ�.

���:
	- ���ؽ� ���̴� �� �ȼ� ���̴� ���� �� ����
	- ������ ���� ���� ������ �׸��� ����� �Է� ���ձ� ����
	- GPU�� ������ ���� �����͸� CPU���� ����
*/
class BitmapClass
{
public:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

	// ���� ������ ���� �����͸� �����ϴ� ���� ����
	struct ModelType
	{
		float x, y, z; // vertex
		float tu, tv; // texture uv
	};

	BitmapClass();
	~BitmapClass();

	int GetIndexCount() const { return m_indexCount; }
	ID3D11ShaderResourceView* GetTexture() const { return m_texture->GetTexture(); }
	void SetRenderLocation(int x, int y) { m_renderX = x; m_renderY = y; }

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, char* textureFilename, int renderX, int renderY);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext);

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
	int m_renderX;
	int m_renderY;
	int m_prevPosX;
	int m_prevPosY;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	TextureClass* m_texture;
};


#endif // !__DEFINE_BITMAPCLASS_H_
