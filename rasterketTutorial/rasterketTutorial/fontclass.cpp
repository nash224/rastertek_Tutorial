//***************************************************************************************
// fontclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "fontclass.h"

using namespace DirectX;

FontClass::FontClass()
{
	m_Font = 0;
	m_Texture = 0;

	m_fontHeight = 0.0f;
	m_spaceSize = 0;
}

FontClass::~FontClass()
{

}

bool FontClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int fontChoice)
{
	char fontFilename[128];
	char fontTextureFilename[128];
	bool result;

	switch (fontChoice)
	{
	case 0:
	{
		strcpy_s(fontFilename, "../rasterketTutorial/Engine/data/font/font01.txt");
		strcpy_s(fontTextureFilename, "../rasterketTutorial/Engine/data/font/font01.tga");
		m_fontHeight = 32.0f;
		m_spaceSize = 3;
		break;
	}
	default:
	{
		strcpy_s(fontFilename, "../rasterketTutorial/Engine/data/font/font01.txt");
		strcpy_s(fontTextureFilename, "../rasterketTutorial/Engine/data/font/font01.tga");
		m_fontHeight = 32.0f;
		m_spaceSize = 3;
		break;
	}
	}

	// 기하 도형의 정점 정보를 불러온다.
	result = LoadFontData(fontFilename);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, deviceContext, fontTextureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
}

void FontClass::BuldVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, letter;

	vertexPtr = (VertexType*)vertices;

	numLetters = (int)strlen(sentence);

	index = 0;

	for (int i = 0; i < numLetters; i++)
	{
		// 배열 0번에 들어갈 아스키코드 문자
		letter = (int)sentence[i] - 32;

		if (letter == 0)
		{
			drawX = drawX + m_spaceSize;
		}
		else
		{
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f); // left top
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f); 
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY - m_fontHeight, 0.0f); // right bottom
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f); 
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, drawY - m_fontHeight, 0.0f); // left bottom
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f); 
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f); // left top
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f); // right top
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY - m_fontHeight, 0.0f); // right botom
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			drawX = drawX + m_Font[letter].size + 1.f;
		}
	}
}

int FontClass::GetSentencePixelLength(char* sentence)
{
	int pixelLength = 0;
	int numLetters = (int)strlen(sentence);

	int letter;
	for (int i = 0; i < numLetters; i++)
	{
		letter = (int)sentence[i] - 32;

		if (letter == 0)
		{
			pixelLength += m_spaceSize;
		}
		else
		{
			pixelLength += m_Font[letter].size + 1;
		}
	}

	return pixelLength;
}

int FontClass::GetFontHeight()
{
	return (int)(m_fontHeight);
}

bool FontClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	m_Texture = new TextureClass;
	result = m_Texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void FontClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}

bool FontClass::LoadFontData(char* filename)
{
	std::ifstream fin;
	char temp;

	m_Font = new FontType[95];

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	for (int i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}
}