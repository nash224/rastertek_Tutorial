//***************************************************************************************
// textureclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "textureclass.h"


TextureClass::TextureClass()
{
	m_targaData = 0;
	m_texture = 0;
	m_textureView = 0;
	m_width = 0;
	m_height = 0;
	m_channelCount = 0;
}

TextureClass::~TextureClass()
{

}

// 텍스처 로드 및 자원 할당, 텍스처 view 객체 생성
bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	HRESULT hResult;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	unsigned int rowPitch;

	result = LoadTarga(filename);
	if (!result)
	{
		goto JumpError;
	}

	// 텍스처 적재하는 방법 및 텍스처를 사용 방법 서술
	textureDesc.Height = m_height;
	textureDesc.Width = m_width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32 bit per texel
	textureDesc.SampleDesc.Count = 1; // 멀티 샘플링 x
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT; // CPU에서 쓰기 불가 (UpdageSubResource는 예외)
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; // 렌더타겟과 쉐이더에서 접근 가능
	textureDesc.CPUAccessFlags = 0; // CPU에서 접근 불가
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS; // 텍스처 리소스에서 mip level 생성 가능

	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult))
	{
		goto JumpError;
	}

	// 텍스처 가로 데이터 크기
	rowPitch = (m_width * 4) * sizeof(unsigned char);

	// 텍스처 데이터를 CPU에서 GPU로 복사를 수행한다.
	// map, unmap 인터페이스를 사용하는 것보다 복사비용은 크지만,
	// 캐쉬 우선권이 보장되기 때문에 매 프레임마다 UpdateSubresource으로
	// 적재한 텍스처를 불러올 경우 map, unmap보다 빠르게 로드된다.
	deviceContext->UpdateSubresource(
		m_texture,    // 텍스처 버퍼에 대한 포인터
		0,            // 자원의 읽는 지점
		NULL,  
		m_targaData, // 텍스처 자원 원본
		rowPitch,     // 이미지 행 하나의 크기
		0 
	);

	// SRV는 쉐이더에서 텍스처를 조작하는 권한이다.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; // 뷰 객체 종류
	srvDesc.Texture2D.MostDetailedMip = 0; // 모름
	srvDesc.Texture2D.MipLevels = -1; // 텍스처의 수준을 최대로 세팅

	// SRV 생성
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hResult))
	{
		goto JumpError;
	}

	// 텍스처 mipmap 생성
	deviceContext->GenerateMips(m_textureView);
	
	// raw data를 사용할 일이 없으므로 자원을 반환한다.
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = 0;
	}

	return true;

JumpError:

	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = 0;
	}

	return false;
}

void TextureClass::Shutdown()
{
	if (m_textureView)
	{
		m_textureView->Release();
		m_textureView = 0;
	}

	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = 0;
	}
}

bool TextureClass::LoadTarga(char* filename)
{
	int error, bpp, imageSize, textureSize;
	FILE* filePtr = nullptr;
	TargaHeader targaFileHeader;
	unsigned int count;
	unsigned char* targaImage = nullptr;

	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		goto errorScope;
	}

	// 천체 항목의 수가 1이 아니라면 파일을 읽어들이지 못했다는 의미
	count = (unsigned int)fread(
		&targaFileHeader,    // 파일의 정보
		sizeof(TargaHeader), // 항목 크기
		1,                   // 항목 수
		filePtr              // 전달받을 파일 데이터에 대한 포인터
	);
	if (count != 1)
	{
		goto errorScope;
	}

	m_height = (int)targaFileHeader.height;
	m_width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	m_channelCount = bpp / 8;

	// 상하 반전되어있는 이미지를 뒤집어 읽어온다.
	imageSize = m_width * m_height * m_channelCount;
	textureSize = m_width * m_height * 4;

	// 이미지 raw data 
	targaImage = new unsigned char[imageSize];

	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		goto errorScope;
	}

	m_targaData = new unsigned char[textureSize];

	if (24 == bpp)
	{
		CopyTarga24Bit(targaImage, imageSize, m_channelCount, m_targaData);

	}
	else
	{
		CopyTarga32Bit(targaImage, imageSize, m_channelCount, m_targaData);
	}

	if (targaImage)
	{
		delete[] targaImage;
		targaImage = 0;
	}

	if (filePtr)
	{
		fclose(filePtr);
		filePtr = 0;
	}

	return true;

errorScope:

	if (targaImage)
	{
		delete[] targaImage;
		targaImage = 0;
	}

	if (filePtr)
	{
		fclose(filePtr);
		filePtr = 0;
	}

	return false;
}

// 24bit 형식인 tga 이미지에 대한 정보와 텍스처 로드
// tga 이미지를 얻어올 경우, 이미지를 반전시켜서 로드한다.
void TextureClass::CopyTarga24Bit(unsigned char* targaImage, int ImageSize, int channelCount, unsigned char* buffer) const
{
	int index, i, j, k;

	// 복사받을 텍셀 인덱스
	index = 0;

	// 복사할 텍스처의 텍셀 인덱스
	// 텍스처 픽셀 데이터 마지막 줄부터 시작
	k = (m_width * m_height * channelCount) - (m_width * channelCount);

	for (j = 0; j < m_height; j++)
	{
		for (i = 0; i < m_width; i++)
		{
			buffer[index + 0] = targaImage[k + 2]; // Red
			buffer[index + 1] = targaImage[k + 1]; // Green
			buffer[index + 2] = targaImage[k + 0]; // Blue
			buffer[index + 3] = 255; // alpha

			k += channelCount;
			index += 4;
		}

		k -= (m_width * (channelCount * 2));
	}
}


// 32bit 형식인 tga 이미지에 대한 정보와 텍스처 로드
// tga 이미지를 얻어올 경우, 이미지를 반전시켜서 로드한다.
void TextureClass::CopyTarga32Bit(unsigned char* targaImage, int ImageSize, int channelCount, unsigned char* buffer) const
{
	int index, i, j, k;

	// 복사받을 텍셀 인덱스
	index = 0;

	// 복사할 텍스처의 텍셀 인덱스
	// 텍스처 픽셀 데이터 마지막 줄부터 시작
	k = (m_width * m_height * channelCount) - (m_width * channelCount);

	for (j = 0; j < m_height; j++)
	{
		for (i = 0; i < m_width; i++)
		{
			buffer[index + 0] = targaImage[k + 2]; // Red
			buffer[index + 1] = targaImage[k + 1]; // Green
			buffer[index + 2] = targaImage[k + 0]; // Blue
			buffer[index + 3] = targaImage[k + 3]; // Alhpa

			k += channelCount;
			index += 4;
		}

		k -= (m_width * (channelCount * 2));
	}
}
