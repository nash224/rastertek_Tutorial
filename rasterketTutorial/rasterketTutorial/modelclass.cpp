//***************************************************************************************
// modelclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "modelclass.h"

using namespace DirectX;

ModelClass::ModelClass()
{
	m_vertexCount = 0;
	m_indexCount = 0;

	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffers(deviceContext);
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	HRESULT result;
	VertexType* vertices = nullptr;
	unsigned long* indices = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;

	XMFLOAT4 Color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// ������ �����ϴ� ���� ��
	m_vertexCount = 4;
	// ���� �ε��� ��
	m_indexCount = 6;

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

	// ���� �̷�� ���� ��ġ, ���� ���� �Է�
	// �簢�� �� ��ġ:	0  1
	//					2  3
	vertices[0].position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertices[0].color = Color;

	vertices[1].position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertices[1].color = Color;

	vertices[2].position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertices[2].color = Color;

	vertices[3].position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertices[3].color = Color;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 3;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 2;

	vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;                // gpu���� �б� �� ���� 
	vertexBufferDesc.ByteWidth           = sizeof(VertexType) * m_vertexCount; // ������ �̷�� ���� ��ü ũ��
	vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;           // ���ؽ� ���۷� ���
	vertexBufferDesc.CPUAccessFlags      = 0;                                  // cpu ���� �Ұ�
	vertexBufferDesc.MiscFlags           = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem          = vertices;
	vertexData.SysMemPitch      = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���ؽ� ���� ����
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
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

void ModelClass::ShutdownBuffers()
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
void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
