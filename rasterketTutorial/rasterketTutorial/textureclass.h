//***************************************************************************************
// textureclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_TEXTURECLASS_H_
#define __DEFINE_TEXTURECLASS_H_

// 화면에 그림을 출력하기 위한 그래픽 자원
// 이를 텍스처라고 하는데 텍스처는 2차원으로 이루어진 색상 채널로 이루어져 있다.
// 대부분의 텍스처는 RGBA 4개의 색상 채널로 구성되어 있다.
// 
// 3차원 상에 존재하는 폴리곤에 텍스처를 입힐려면 기하 도형의 uv와 텍스처 좌표를 맵핑시켜야 한다.
// 이때 사용하는 텍스처 좌표를 텍셀 좌표라고 한다.
// 텍셀 좌표는 가로축 U, 세로축 V, 깊이축 W로 구성되어 있고 
// 

//---------------------------------------------------------------------------------------
// INCLUDES 
//---------------------------------------------------------------------------------------
#include <d3d11.h>
#include <stdio.h>

/*
목적:		
	텍스처의 정보를 담고 있는 클래스
	외부 이미지 파일을 불러와 GPU에 적재하고, 
	쉐이더에서 사용할 수 있도록 텍스처 view를 제공한다.

기능:
	- 텍스처 파일 로딩
	- GPU 리소스 생성 및 해제
	- 쉐이더 리서스 뷰  제공
*/
class TextureClass
{
public:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width; // 이미지 가로 크기
		unsigned short height; // 이미지 세로 크기
		unsigned char bpp; // // 텍셀 크기
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
	 * @brief		Initialize		텍스처 파일 로딩, 쉐이더 리소스 뷰 생성
	 *
	 * @param		void		device			GPU 자원 생성 핸들
	 * @param		void		deviceContext	랜더 파이프라인 인터페이스
	 * @param		void		filename		이미지 파일 경로
	 *
	 * @return		bool		GPU 리소스 생성 유무
	 */
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	void Shutdown();

private:

	/**
	 * @brief		LoadTarga32Bit		targa형식의 이미지 로드
	 *
	 * @param		filename		파일 이미지 경로
	 *
	 * @return		bool		이미지 파일 로딩 유무
	 */
	bool LoadTarga(char* filename);
	void CopyTarga24Bit(unsigned char* targaImage, int ImageSize, int channelCount, unsigned char* buffer) const;
	void CopyTarga32Bit(unsigned char* targaImage, int ImageSize, int channelCount, unsigned char* buffer) const;

private:
	unsigned char* m_targaData; // 텍스처 raw data에 대한 포인터
	ID3D11Texture2D* m_texture;  // 텍스처 자원에 대한 인터페이스
	ID3D11ShaderResourceView* m_textureView; // 쉐이더에서 텍스처를 조작할 수 있는 권한
	int m_width, m_height, m_channelCount;

};


#endif // !__DEFINE_TEXTURECLASS_H_
