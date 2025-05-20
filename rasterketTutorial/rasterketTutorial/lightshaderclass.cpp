//***************************************************************************************
// LightShaderClass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "LightShaderClass.h"

using namespace DirectX;

// ȯ�汤
// 
// �¾籤�� �ǳ��� �������� ���� ����Ǽ� ���´�.
// �̷��� ���� ����Ǿ� ������ ȿ���� ȯ�汤�̶�� �Ѵ�.
// 
// ȯ�汤(ambient light)�� ������ ������ ��� ��鿡 ���� ���� �ּ�ġ�� ���ϸ� �ȴ�.
// �̷� ȿ���� ������ ��� �������� ������ ���� ����� ������ �� �ִ�.
// 
// ȯ�汤�� ����ڰ� ���� �����ϴ�.
//

LightShaderClass::LightShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
	m_lightColorBuffer = 0;
	m_lightPositionBuffer = 0;
}

LightShaderClass::~LightShaderClass()
{

}

bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	wchar_t vsFileName[128];
	wchar_t psFileName[128];
	int error;

	// �������� hlsl������ ��θ� �����ڵ�� ��ȯ
	error = wcscpy_s(vsFileName, 128, L"../rasterketTutorial/Engine/lightVS.hlsl");
	if (error != 0)
	{
		return false;
	}

	error = wcscpy_s(psFileName, 128, L"../rasterketTutorial/Engine/lightPS.hlsl");
	if (error != 0)
	{
		return false;
	}

	result = InitializeShader(device, hwnd, vsFileName, psFileName);
	if (!result)
	{
		return false;
	}

	return true;
}

void LightShaderClass::Shutdown()
{
	ShutdownShader();
}

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, XMFLOAT4 diffuseColor[], XMFLOAT4 lightPosition[])
{
	bool result;

	// ������� ����
	result = SetShaderParamters(deviceContext, 
		worldMatrix, viewMatrix, projectionMatrix,
		texture, diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	// �Է� ���̾ƿ�, ���̴� ���� �� Draw Call
	RenderShader(deviceContext, indexCount);

	return true;
}

// �Է� ���̾ƿ�, ��� ����, ���̴� �������̽��� ����
bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	ID3D10Blob* errorMessage = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3] = {};
	unsigned int numElements;
	D3D11_BUFFER_DESC MatrixBufferDesc = {};
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightColorBufferDesc = {};
	D3D11_BUFFER_DESC lightPositionBufferDesc = {};

	// ���̴� �ڵ� ��ȿ�� ������
	result = D3DCompileFromFile(
		vsFilename,                     // ���̴� �ڵ尡 ���Ե� ���� �̸�
		NULL, NULL,                     // ��
		"LightVertexShader",            // ���̴� �ڵ� ������
		"vs_5_0",                       // ���̴� �� ����
		D3D10_SHADER_ENABLE_STRICTNESS, // ������ �ɼ�
		0,                              // ������ �ɼ� 2
		&vertexShaderBuffer,            // ������ ���޿� ��ü, ���̴� ��ü�� ������ �� ���
		&errorMessage                   // ���̴� ���� �޼���
	);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);

			// �ʿ����� ���� �޼��� ���۴� �����Ѵ�.
			errorMessage->Release();
			errorMessage = 0;
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing ShaderFile.", MB_OK);
		}

		return false;
	}

	// �ȼ� ���̴� ������
	result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);

			// �ʿ����� ���� �޼��� ���۴� �����Ѵ�.
			errorMessage->Release();
			errorMessage = 0;
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing ShaderFile.", MB_OK);
		}

		return false;
	}

	// ���ؽ� ���̴� ����
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// �ȼ� ���̴� ����
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// gpu���� ����� ����ü�� ��ġ�ϵ��� �ۼ��Ѵ�.
	polygonLayout[0].SemanticName = "POSITION";                  // gpu���� �ν��� ���� �ø�ƽ 
	polygonLayout[0].SemanticIndex = 0;                           // ���� �̸��� �ø�ƽ�� ��쿡 ���, �� ������ ����
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // ���� ���� ����, 12byte floatŸ�� ���
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;                           // ������ ������, gpu�� �ö� �����ʹ� �Ѳ����� ���޵Ǳ� ������ ���� ���� offset�� �˷������
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // ���ؽ��� ���, �ٸ� ���� �ν��Ͻ̿� ���
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";	// �ؼ� ��ǥ �ø�ƽ ���
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT; // uv�� 2���� ��ǥ�� �䱸��
	polygonLayout[1].InputSlot = 0; // ���� 0��
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";	// ������ �븻 ����
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT; // ���� ����(x,y,z)�� float3 ���
	polygonLayout[2].InputSlot = 0; // ���� 0��
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// �Է� ���̾ƿ� ����
	result = device->CreateInputLayout(
		polygonLayout,                          // �Է� ������ ������ �迭
		numElements,                            // �Է� ������ ���� ��
		vertexShaderBuffer->GetBufferPointer(), // �����ϵ� ���̴� �ڵ忡 ���� ������
		vertexShaderBuffer->GetBufferSize(),    // �����ϵ� ���̴� �ڵ忡 ���� ũ��
		&m_layout                               // ���޹��� ������
	);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// ��� ���� ����
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;             // ��Ÿ�ӿ��� ����ϰڴ�.
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);    // ����ü ũ��
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // ��� ���۷� ���
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu ���� ����
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;                 // ��Ʈ��ó�� ���ۿ� ���

	result = device->CreateBuffer(&MatrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �ؽ�ó�� �ȼ��� �����ϴ� ��� ����
	// �����Ͷ������� ��ģ ���� ������ �ȼ��� 
	// ���÷��� �ؽ�ó�� �ȼ��� �����Ͽ� �˷��ش�.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// �ؽ�ó�� ���ø��� �� ���͸� ���
															// D3D11_FILTER_MIN_MAG_MIP_LINEAR�� ������ ���, 
															// ���÷��� ������������ ������ ����.

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	// D3D11_TEXTURE_ADDRESS_WRAP:   �ؽ�ó ��ǥ[0,1]�� �ѱ� ���, uv �ݺ�
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;	// D3D11_TEXTURE_ADDRESS_MIRROR: �ؽ�ó ��ǥ[0,1]�� �ѱ� ���, uv ����
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;  // D3D11_TEXTURE_ADDRESS_CLAMP:  �ؽ�ó ��ǥ�� [0,1]�� clamp ó��
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1; // �̹漺 ���ÿ��� ���� ������ ��
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0.0f; // �׵θ� ��
	samplerDesc.BorderColor[1] = 0.0f; // �׵θ� ��
	samplerDesc.BorderColor[2] = 0.0f; // �׵θ� ��
	samplerDesc.BorderColor[3] = 0.0f; // �׵θ� ��
	samplerDesc.MinLOD = 0;  // mipmap ����, ���� ���� ǰ��
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // ���� ���� ǰ��

	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	// �ȼ� ���̴����� ���� ���ݻ籤
	lightColorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;             // ��Ÿ�� �߿� gpu�� �����ؾ� ��
	lightColorBufferDesc.ByteWidth = sizeof(LightColorBufferType);    // ����ü ũ��
	lightColorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // ������ �뵵
	lightColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu���� ���� ����
	lightColorBufferDesc.MiscFlags = 0;
	lightColorBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightColorBufferDesc, NULL, &m_lightColorBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// �ȼ� ���̴����� ���� light ��� ����
	lightPositionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;             // cpu���� ���� ����
	lightPositionBufferDesc.ByteWidth = sizeof(LightPositionBufferType);     // ����ü ũ��
	lightPositionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // ��� ���۷� ���
	lightPositionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu���� ���� ����
	lightPositionBufferDesc.MiscFlags = 0;
	lightPositionBufferDesc.StructureByteStride = 0;                 // ��Ʈ��ó�� ���ۿ� ���

	result = device->CreateBuffer(&lightPositionBufferDesc, NULL, &m_lightPositionBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void LightShaderClass::ShutdownShader()
{
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	if (m_lightPositionBuffer)
	{
		m_lightPositionBuffer->Release();
		m_lightPositionBuffer = 0;
	}

	if (m_lightColorBuffer)
	{
		m_lightColorBuffer->Release();
		m_lightColorBuffer = 0;
	}
}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();

	// ������ ���� �޼����� ����Ѵ�.
	fout.open("shader-error.txt");

	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	MessageBox(hwnd, L"Error compile Shader. Check Shader-error.txt for message.", shaderFilename, MB_OK);
}

// ���̴� �ڵ忡�� ����� ��� ���� ����
// �ȼ� ���̴��� ���� �ؽ�ó ���Կ� ����� �ؽ�ó ����
bool LightShaderClass::SetShaderParamters(ID3D11DeviceContext* deviceContext,
	DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4 diffuseColor[], DirectX::XMFLOAT4 lightPosition[])
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightColorBufferType* dataPtr1;
	LightPositionBufferType* dataPtr2;
	unsigned int bufferNumber;

	// d3d11 ���Ŀ� �´� ��Ʈ���� �������� ��ȯ
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	{
		// cpu���� �ڿ��� gpu�� ���� ��, gpu�� �����ϸ� ������ �߻��Ѵ�.
		// gpu�� �ش� �ڿ��� �������� ���ϵ��� Lock�� �Ǵ�
		result = deviceContext->Map(
			m_matrixBuffer,
			0,
			D3D11_MAP_WRITE_DISCARD, // cpu���� ���� ������ gpu�� �����Ǿ� �ִ� �����ʹ� ��� ���� ����
			0,
			&mappedResource
		);
		if (FAILED(result))
		{
			return false;
		}

		// ���⿡ ���� ��� �����͸� �����Ѵ�.
		dataPtr = (MatrixBufferType*)mappedResource.pData;
		dataPtr->world = worldMatrix;
		dataPtr->view = viewMatrix;
		dataPtr->projection = projectionMatrix;

		// �����͸� ���� �����ߴٸ� Unlock ����
		// �׳� ���� ���� ��, gpu�� Deadlock
		deviceContext->Unmap(m_matrixBuffer, 0);

		bufferNumber = 0;

		// ��� ���� ����
		// ��� ���� 14������ ��� ����
		deviceContext->VSSetConstantBuffers(
			bufferNumber,   // 0�� ���� ���
			1,              // ������� 1�� ���
			&m_matrixBuffer // ���� �������̽�
		);
	}

	{
		// 0�� ���Կ� texture ����
		deviceContext->PSSetShaderResources(0, 1, &texture);
	}


	{
		// ī�޶���� ������� ����
		result = deviceContext->Map(m_lightColorBuffer, 0, D3D11_MAP_WRITE_DISCARD,
			0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		dataPtr1 = (LightColorBufferType*)mappedResource.pData;
		dataPtr1->diffuseColor[0] = diffuseColor[0];
		dataPtr1->diffuseColor[1] = diffuseColor[1];
		dataPtr1->diffuseColor[2] = diffuseColor[2];
		dataPtr1->diffuseColor[3] = diffuseColor[3];

		deviceContext->Unmap(m_lightColorBuffer, 0);

		bufferNumber = 0;

		// �ȼ� ���̴� ������� 1�� ���Կ� ����
		deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightColorBuffer);
	}

	{
		result = deviceContext->Map(
			m_lightPositionBuffer, 0,
			D3D11_MAP_WRITE_DISCARD,
			0, &mappedResource);
		if (FAILED(result))
		{
			return false;
		}

		// ���⿡ ���� ��� �����͸� �����Ѵ�.
		dataPtr2 = (LightPositionBufferType*)mappedResource.pData;
		dataPtr2->lightPosition[0] = lightPosition[0];
		dataPtr2->lightPosition[1] = lightPosition[1];
		dataPtr2->lightPosition[2] = lightPosition[2];
		dataPtr2->lightPosition[3] = lightPosition[3];

		// �����͸� ���� �����ߴٸ� Unlock ����
		// �׳� ���� ���� ��, gpu�� Deadlock
		deviceContext->Unmap(m_lightPositionBuffer, 0);

		bufferNumber = 1;

		// ��� ���� ����
		deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_lightPositionBuffer);
	}

	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// �Է� ���̾ƿ� ����
	deviceContext->IASetInputLayout(m_layout);

	// ���ؽ� �� �ȼ� ���̴� ����
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// �ȼ� ���̴� ���÷� 0�� ���Կ� ���÷� ����
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// ������ �ε��� ����ŭ �׸���.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}