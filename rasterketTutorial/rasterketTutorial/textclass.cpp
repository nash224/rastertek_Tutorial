//***************************************************************************************
// textclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "textclass.h"

using namespace DirectX;

TextClass::TextClass()
{
	m_vertexCount = 0;
	m_indexCount = 0;
	m_screenWidth = 0;
	m_screenHeight = 0;

	m_maxLength = 0;

	m_vertexBuffer = 0;
	m_indexBuffer = 0;

	m_pixelColor = {};
}

TextClass::~TextClass()
{

}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight,
	int maxLength, FontClass* Font, char* text, int positionX, int positionY,
	float red, float green, float blue)
{
	bool result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_maxLength = maxLength;

	result = InitializeBuffers(device, deviceContext, Font, text, positionX, positionY, red, green, blue);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	ShutdownBuffers();
}

void TextClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

bool TextClass::UpdateText(ID3D11DeviceContext* deviceContext, FontClass* Font, 
	char* text, int positionX, int positionY, float red, float green, float blue)
{
	int numLetters;
	VertexType* vertices = nullptr;
	float drawX, drawY;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;

	m_pixelColor = XMFLOAT4(red, green, blue, 1.0f);

	numLetters = (int)strlen(text);
	if (numLetters > m_maxLength)
	{
		goto JumpError;
	}

	vertices = new VertexType[m_vertexCount];
	memset(vertices, 0, sizeof(VertexType) * m_vertexCount);

	drawX = (float)(positionX - (m_screenWidth / 2));
	drawY = (float)((m_screenHeight / 2) - positionY);

	// ���� ���� �ʱ�ȭ
	Font->BuldVertexArray((void*)vertices, text, drawX, drawY);

	// GPU�� ���� ���� ����
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		goto JumpError;
	}

	verticesPtr = (VertexType*)mappedResource.pData;

	memcpy_s(verticesPtr, sizeof(VertexType) * m_vertexCount, (void*)vertices, sizeof(VertexType) * m_vertexCount);

	deviceContext->Unmap(m_vertexBuffer, 0);

	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	return true;

JumpError:

	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}

	return false;
}

bool TextClass::InitializeBuffers(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FontClass* Font, 
	char* text, int positionX, int positionY, float red, float green, float blue)
{
	VertexType* vertices = nullptr;
	unsigned long* indices = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT hresult;
	bool result;

	m_vertexCount = m_maxLength * 6;
	m_indexCount = m_vertexCount;

	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		goto failed;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		goto failed;
	}

	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	for (int i = 0; i < m_vertexCount; i++)
	{
		indices[i] = i;
	}

	vertexBufferDesc.Usage               = D3D11_USAGE_DYNAMIC; 
	vertexBufferDesc.ByteWidth           = sizeof(VertexType) * m_vertexCount; // ������ �̷�� ���� ��ü ũ��
	vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;           // ���ؽ� ���۷� ���
	vertexBufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags           = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem          = vertices;
	vertexData.SysMemPitch      = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���ؽ� ���� ����
	hresult = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(hresult))
	{
		goto failed;
	}

	indexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;                // gpu���� �б� �� ���� 
	indexBufferDesc.ByteWidth           = sizeof(unsigned long) * m_indexCount; // ������ �̷�� ���� ��ü ũ��
	indexBufferDesc.BindFlags           = D3D11_BIND_INDEX_BUFFER;           // ���ؽ� ���۷� ���
	indexBufferDesc.CPUAccessFlags      = 0;                                  // cpu ���� �Ұ�
	indexBufferDesc.MiscFlags           = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem          = indices;
	indexData.SysMemPitch      = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���� ����
	hresult = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(hresult))
	{
		goto failed;
	}

	result = UpdateText(deviceContext, Font, text, positionX, positionY, red, green, blue);
	if (!result)
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

void TextClass::ShutdownBuffers()
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

// ������ �����ϴ� ���۸� GPU�� �����Ѵ�.
// ������ ���� �ܰ�(input assembler)�� ��ġ��, ������ �׸��� ����� �����Ѵ�.
void TextClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}