//***************************************************************************************
// textureclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_TEXTURECLASS_H_
#define __DEFINE_TEXTURECLASS_H_

// ȭ�鿡 �׸��� ����ϱ� ���� �׷��� �ڿ�
// �̸� �ؽ�ó��� �ϴµ� �ؽ�ó�� 2�������� �̷���� ���� ä�η� �̷���� �ִ�.
// ��κ��� �ؽ�ó�� RGBA 4���� ���� ä�η� �����Ǿ� �ִ�.
// 
// 3���� �� �����ϴ� �����￡ �ؽ�ó�� �������� ���� ������ uv�� �ؽ�ó ��ǥ�� ���ν��Ѿ� �Ѵ�.
// �̶� ����ϴ� �ؽ�ó ��ǥ�� �ؼ� ��ǥ��� �Ѵ�.
// �ؼ� ��ǥ�� ������ U, ������ V, ������ W�� �����Ǿ� �ְ� 
// 

//---------------------------------------------------------------------------------------
// INCLUDES 
//---------------------------------------------------------------------------------------
#include <d3d11.h>
#include <stdio.h>

/*
����:		
	�ؽ�ó�� ������ ��� �ִ� Ŭ����
	�ܺ� �̹��� ������ �ҷ��� GPU�� �����ϰ�, 
	���̴����� ����� �� �ֵ��� �ؽ�ó view�� �����Ѵ�.

���:
	- �ؽ�ó ���� �ε�
	- GPU ���ҽ� ���� �� ����
	- ���̴� ������ ��  ����
*/
class TextureClass
{
public:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width; // �̹��� ���� ũ��
		unsigned short height; // �̹��� ���� ũ��
		unsigned char bpp; // // �ؼ� ũ��
		unsigned char data2;
	};

	// constructor & destructor
	TextureClass();
	~TextureClass();

	// delete function
	TextureClass(const TextureClass& _Other) = delete;
	TextureClass(TextureClass&& _Other) noexcept = delete;
	TextureClass& operator=(const TextureClass& _Other) = delete;
	TextureClass& operator=(TextureClass&& _Other) noexcept = delete;

	ID3D11ShaderResourceView* GetTexture() const { return m_textureView; }
	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }


	/**
	 * @brief		Initialize		�ؽ�ó ���� �ε�, ���̴� ���ҽ� �� ����
	 *
	 * @param		void		device			GPU �ڿ� ���� �ڵ�
	 * @param		void		deviceContext	���� ���������� �������̽�
	 * @param		void		filename		�̹��� ���� ���
	 *
	 * @return		bool		GPU ���ҽ� ���� ����
	 */
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	void Shutdown();

private:

	/**
	 * @brief		LoadTarga32Bit		targa������ �̹��� �ε�
	 *
	 * @param		filename		���� �̹��� ���
	 *
	 * @return		bool		�̹��� ���� �ε� ����
	 */
	bool LoadTarga(char* filename);
	void CopyTarga24Bit(unsigned char* targaImage, int ImageSize, int channelCount, unsigned char* buffer) const;
	void CopyTarga32Bit(unsigned char* targaImage, int ImageSize, int channelCount, unsigned char* buffer) const;

private:
	unsigned char* m_targaData; // �ؽ�ó raw data�� ���� ������
	ID3D11Texture2D* m_texture;  // �ؽ�ó �ڿ��� ���� �������̽�
	ID3D11ShaderResourceView* m_textureView; // ���̴����� �ؽ�ó�� ������ �� �ִ� ����
	int m_width, m_height, m_channelCount;

};


#endif // !__DEFINE_TEXTURECLASS_H_
