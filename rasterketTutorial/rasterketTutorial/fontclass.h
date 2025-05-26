//***************************************************************************************
// fontclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_FONTCLASS_H_
#define __DEFINE_FONTCLASS_H_


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
// Class name: FontClass
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
class FontClass
{
public:
	// �ؽ�ó uv, ũ�� ������
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};

public:
	FontClass();
	~FontClass();

	ID3D11ShaderResourceView* GetTexture() const { return m_Texture->GetTexture(); }

	/**
	 * @brief		Initialize		�������� �ʿ��� ���� ��Ʈ ������ �ε�
	 *
	 * @param		device			���۸� �����ϱ� ���� d3d �ڵ�
	 * @param		deviceContext	���� ���������� �ڵ�
	 * @param		fontChoice		����� ��Ʈ Ÿ��
	 *
	 * @return		void
	 *
	 * @warning		null
	 */
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int fontChoice);
	void Shutdown();

	/**
	 * @brief		BuldVertexArray		�� ���� ���� �ʱ�ȭ
	 *
	 * @param		vertices			������ �����ϴ� ���� �迭 ������
	 * @param		sentence			�׸� ����
	 * @param		drawX, drawY		���� ��� ��ġ
	 *
	 * @return		void
	 *
	 * @warning		null
	 */
	void BuldVertexArray(void* vertices, char* sentence, float drawX, float drawY);
	int GetSentencePixelLength(char* sentence);
	int GetFontHeight();

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


	/**
	 * @brief		LoadModel		�ؽ�Ʈ ����� ���Ͽ��� ���� ������ ���� ������ �ε�
	 *
	 * @param		filename		���� �����͸� ������ ���� �̸�
	 *
	 * @return		bool			�ε� ����
	 * 
	 * @warning		InitalizeBuffer�� ȣ���ϱ� ���� ȣ��Ǿ�� ��
	 */
	bool LoadFontData(char* filename);

	/**
	 * @brief		ReleaseModel		����� ���� ���� ���� ��ü ����
	 *
	 * @return		void
	 */
	void ReleaseFontData();

private:
	FontType* m_Font;
	TextureClass* m_Texture;

	float m_fontHeight;
	int m_spaceSize;		// ������� ����

};


#endif // !__DEFINE_FONTCLASS_H_
