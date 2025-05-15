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

// �ؽ�ó �ε� �� �ڿ� �Ҵ�, �ؽ�ó view ��ü ����
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

	// �ؽ�ó �����ϴ� ��� �� �ؽ�ó�� ��� ��� ����
	textureDesc.Height = m_height;
	textureDesc.Width = m_width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32 bit per texel
	textureDesc.SampleDesc.Count = 1; // ��Ƽ ���ø� x
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT; // CPU���� ���� �Ұ� (UpdageSubResource�� ����)
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; // ����Ÿ�ٰ� ���̴����� ���� ����
	textureDesc.CPUAccessFlags = 0; // CPU���� ���� �Ұ�
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS; // �ؽ�ó ���ҽ����� mip level ���� ����

	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult))
	{
		goto JumpError;
	}

	// �ؽ�ó ���� ������ ũ��
	rowPitch = (m_width * 4) * sizeof(unsigned char);

	// �ؽ�ó �����͸� CPU���� GPU�� ���縦 �����Ѵ�.
	// map, unmap �������̽��� ����ϴ� �ͺ��� �������� ũ����,
	// ĳ�� �켱���� ����Ǳ� ������ �� �����Ӹ��� UpdateSubresource����
	// ������ �ؽ�ó�� �ҷ��� ��� map, unmap���� ������ �ε�ȴ�.
	deviceContext->UpdateSubresource(
		m_texture,    // �ؽ�ó ���ۿ� ���� ������
		0,            // �ڿ��� �д� ����
		NULL,  
		m_targaData, // �ؽ�ó �ڿ� ����
		rowPitch,     // �̹��� �� �ϳ��� ũ��
		0 
	);

	// SRV�� ���̴����� �ؽ�ó�� �����ϴ� �����̴�.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; // �� ��ü ����
	srvDesc.Texture2D.MostDetailedMip = 0; // ��
	srvDesc.Texture2D.MipLevels = -1; // �ؽ�ó�� ������ �ִ�� ����

	// SRV ����
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hResult))
	{
		goto JumpError;
	}

	// �ؽ�ó mipmap ����
	deviceContext->GenerateMips(m_textureView);
	
	// raw data�� ����� ���� �����Ƿ� �ڿ��� ��ȯ�Ѵ�.
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

	// õü �׸��� ���� 1�� �ƴ϶�� ������ �о������ ���ߴٴ� �ǹ�
	count = (unsigned int)fread(
		&targaFileHeader,    // ������ ����
		sizeof(TargaHeader), // �׸� ũ��
		1,                   // �׸� ��
		filePtr              // ���޹��� ���� �����Ϳ� ���� ������
	);
	if (count != 1)
	{
		goto errorScope;
	}

	m_height = (int)targaFileHeader.height;
	m_width = (int)targaFileHeader.width;
	bpp = (int)targaFileHeader.bpp;

	m_channelCount = bpp / 8;

	// ���� �����Ǿ��ִ� �̹����� ������ �о�´�.
	imageSize = m_width * m_height * m_channelCount;
	textureSize = m_width * m_height * 4;

	// �̹��� raw data 
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

// 24bit ������ tga �̹����� ���� ������ �ؽ�ó �ε�
// tga �̹����� ���� ���, �̹����� �������Ѽ� �ε��Ѵ�.
void TextureClass::CopyTarga24Bit(unsigned char* targaImage, int ImageSize, int channelCount, unsigned char* buffer) const
{
	int index, i, j, k;

	// ������� �ؼ� �ε���
	index = 0;

	// ������ �ؽ�ó�� �ؼ� �ε���
	// �ؽ�ó �ȼ� ������ ������ �ٺ��� ����
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


// 32bit ������ tga �̹����� ���� ������ �ؽ�ó �ε�
// tga �̹����� ���� ���, �̹����� �������Ѽ� �ε��Ѵ�.
void TextureClass::CopyTarga32Bit(unsigned char* targaImage, int ImageSize, int channelCount, unsigned char* buffer) const
{
	int index, i, j, k;

	// ������� �ؼ� �ε���
	index = 0;

	// ������ �ؽ�ó�� �ؼ� �ε���
	// �ؽ�ó �ȼ� ������ ������ �ٺ��� ����
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
