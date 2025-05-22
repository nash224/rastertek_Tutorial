//***************************************************************************************
// spriteclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "spriteclass.h"

using namespace DirectX;

SpriteClass::SpriteClass()
{
	m_vertexCount = 0;
	m_indexCount = 0;

	m_screenWidth = 0;
	m_screenHeight = 0;
	m_bitmapWidth = 0;
	m_bitmapHeight = 0;
	m_renderX = 0;
	m_renderY = 0;
	m_renderScaleX = 1.0f;
	m_renderScaleY = 1.0f;
	m_prevScaleX = 0;
	m_prevScaleY = 0;
	m_prevPosX = 0;
	m_prevPosY = 0;

	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_textures = 0;
	m_textureCount = 0;
	m_cycleTime = 0.0f;
	m_frameTime = 0.0f;
	m_currentTexture = 0;
}

SpriteClass::~SpriteClass()
{

}

bool SpriteClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext,
	int screenWidth, int screenHeight, char* spriteFilename, int renderX, int renderY, float ImageScaleX, float ImageScaleY)
{
	bool result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_renderX = renderX;
	m_renderY = renderY;

	SetRenderScale(ImageScaleX, ImageScaleY);

	m_frameTime = 0;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, deviceContext, spriteFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void SpriteClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

bool SpriteClass::Render(ID3D11DeviceContext* deviceContext)
{
	bool result;

	result = UpdateBuffers(deviceContext);
	if(!result)
	{
		return false;
	}

	RenderBuffers(deviceContext);

	return true;
}

void SpriteClass::Update(float frameTime)
{
	m_frameTime += frameTime;

	if (m_frameTime >= m_cycleTime)
	{
		m_frameTime -= m_cycleTime;

		m_currentTexture++;

		if (m_currentTexture == m_textureCount)
		{
			m_currentTexture = 0;
		}
	}
}

bool SpriteClass::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;
	VertexType* vertices = nullptr;
	unsigned long* indices = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	int i;

	m_prevPosX = -1;
	m_prevPosY = -1;

	m_vertexCount = 6;
	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];
	indices = new unsigned long[m_indexCount];

	// 우선 정점 데이터를 0으로 세팅
	memset(vertices, 0, sizeof(VertexType) * m_vertexCount);

	for (i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// 버퍼를 수정할 수 있는 동적 정점 버퍼 생성
	vertexBufferDesc.Usage               = D3D11_USAGE_DYNAMIC;                // GPU에서 쓰기 가능
	vertexBufferDesc.ByteWidth           = sizeof(VertexType) * m_vertexCount; // 도형을 이루는 점의 전체 크기
	vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;           // 버텍스 버퍼로 사용
	vertexBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;             // cpu에서 쓰기 가능
	vertexBufferDesc.MiscFlags           = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem          = vertices;
	vertexData.SysMemPitch      = 0;
	vertexData.SysMemSlicePitch = 0;

	// 버텍스 버퍼 생성
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		goto failed;
	}

	indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;                // gpu에서 읽기 및 쓰기 
	indexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_indexCount; // 도형을 이루는 점의 전체 크기
	indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;           // 버텍스 버퍼로 사용
	indexBufferDesc.CPUAccessFlags      = 0;                                  // cpu 접근 불가
	indexBufferDesc.MiscFlags           = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem          = indices;
	indexData.SysMemPitch      = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼 생성
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		goto failed;
	}

	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (indices)
	{
		delete[] indices;
		indices = 0;
	}

	return true;

failed:
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (indices)
	{
		delete[] indices;
		indices = 0;
	}

	return false;
}

void SpriteClass::ShutdownBuffers()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}
}

bool SpriteClass::UpdateBuffers(ID3D11DeviceContext* deviceContext)
{
	float left, right, top, bottom, ImageWidth, ImageHeight;
	VertexType* vertices;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* dataPtr;

	// 렌더링될 위치가 동일할 경우 업데이트를 수행하지 않음
	if ((m_prevPosX == m_renderX) && (m_prevPosY == m_renderY) 
		&& (m_prevScaleX == m_renderScaleX) && (m_prevScaleY == m_renderScaleY))
	{
		return true;
	}

	m_prevPosX = m_renderX;
	m_prevPosY = m_renderY;
	m_prevScaleX = m_renderScaleX;
	m_prevScaleY = m_renderScaleY;

	ImageWidth = (float)m_bitmapWidth * m_renderScaleX;
	ImageHeight = (float)m_bitmapHeight * m_renderScaleY;

	vertices = new VertexType[m_vertexCount];


	left   = (float)(m_renderX - m_screenWidth / 2);
	right  = left + ImageWidth;
	top    = (float)(m_screenHeight / 2 - m_renderY);
	bottom = top - ImageHeight;

	vertices[0].position = XMFLOAT3(left, top, 0.0f); // left top
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f); // right bttom 
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f); // left bottom
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(left, top, 0.0f); // left top
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f); // right top
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f); // right bottom
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		goto JumpError;
	}

	dataPtr = (VertexType*)mappedResource.pData;
	
	memcpy_s(dataPtr, sizeof(VertexType) * m_vertexCount, vertices, sizeof(VertexType) * m_vertexCount);

	deviceContext->Unmap(m_vertexBuffer, 0);

	dataPtr = 0;

	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	return true;

JumpError:

	dataPtr = 0;

	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	return false;
}

// 도형을 묘사하는 버퍼를 GPU에 적재한다.
// 도형을 조립 단계(input assembler)를 거치고, 도형을 그리는 방법을 서술한다.
void SpriteClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool SpriteClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	char textureFilename[128];
	std::ifstream fin;
	int i, j;
	char input;
	bool result;

	fin.open(filename);
	if (fin.fail())
	{
		goto JumpError;
	}

	fin >> m_textureCount;

	m_textures = new TextureClass[m_textureCount];

	fin.get(input);

	for (i = 0; i < m_textureCount; i++)
	{
		j = 0;
		fin.get(input);
		while (input != '\n')
		{
			textureFilename[j] = input;
			j++;
			fin.get(input);
		}

		textureFilename[j] = '\0';

		result = m_textures[i].Initialize(device, deviceContext, textureFilename);
		if (!result)
		{
			goto JumpError;
		}
	}

	fin >> m_cycleTime;

	fin.close();

	m_bitmapWidth = m_textures[0].GetWidth();
	m_bitmapHeight = m_textures[0].GetHeight();

	m_currentTexture = 0;

	return true;

JumpError:

	fin.close();
	return false;
}

void SpriteClass::ReleaseTexture()
{
	if (m_textures)
	{
		for (int i = 0; i < m_textureCount; i++)
		{
			m_textures[i].Shutdown();
		}

		delete[] m_textures;
		m_textures = 0;
	}
}