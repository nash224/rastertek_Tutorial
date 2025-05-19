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

	m_texture = 0;
	m_model = 0;
}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename)
{
	bool result;

	// 기하 도형의 정점 정보를 불러온다.
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
	ReleaseModel();
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
	int i;

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

	// 기하 도형의 position, texture uv, normal 정보를 VertexType 배열에 담고,
	// 인덱스 배열은 도형의 순서대로 제공한다. 물론 차후에 수정할 수도 있다.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	vertexBufferDesc.Usage               = D3D11_USAGE_DEFAULT;                // gpu에서 읽기 및 쓰기 
	vertexBufferDesc.ByteWidth           = sizeof(VertexType) * m_vertexCount; // 도형을 이루는 점의 전체 크기
	vertexBufferDesc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;           // 버텍스 버퍼로 사용
	vertexBufferDesc.CPUAccessFlags      = 0;                                  // cpu 접근 불가
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

// 도형을 묘사하는 버퍼를 GPU에 적재한다.
// 도형을 조립 단계(input assembler)를 거치고, 도형을 그리는 방법을 서술한다.
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

bool ModelClass::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	m_texture = new TextureClass;
	result = m_texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = 0;
	}
}

// 텍스처 기반의 기하 도형을 구성하는 정점 데이터를 로드한다.
// 정점의 수는   첫 줄에 기술 
// 정점 데이터는 각 줄마다 기술되어 있으며, 
// 한 줄, 즉 하나의 정점은 순서대로 position, texture uv, normal로 구성된다.
bool ModelClass::LoadModel(char* filename)
{
	std::ifstream fin;
	char input;
	int i;

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> m_vertexCount;

	m_indexCount = m_vertexCount;

	m_model = new ModelType[m_vertexCount];

	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >>  m_model[i].ny >> m_model[i].nz;
	}

	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
}