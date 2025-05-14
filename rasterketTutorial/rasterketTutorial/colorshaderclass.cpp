//***************************************************************************************
// colorshaderclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "colorshaderclass.h"

// ���ؽ� ����
// ��ǻ�Ϳ��� ǥ���ϴ� ��� ��ü�� ���� �ﰢ������ ǥ���ȴ�. 
// �� ������ �̷���� �ִ� �ﰢ���� �׸��� ���ؼ��� 
// ������ ������ ���� �迭�� ���ؽ� ���ۿ� ä�� gpu�� ������ �Ѵ�.

// �ε��� ����
// �ε��� ���۴� ���ؽ� ������ ������ ��� �ִ� �迭�̴�.
// ������ �׸��� ���ؼ� �ʼ� ������ �ƴ����� ���� �ſ� ���� �ӵ���
// ã�� �� �ְ�, �׸��� ������ ������ �� �־� �ظ��ؼ��� ���ؽ� ���ۿ�
// �Բ� gpu�� ������ ����ü�̴�.

// ���ؽ� ���̴�
// ���ؽ� ���̴��� 3d ������ �ִ� ���� �ű�� ���� ���̴� ���α׷����̴�.
// ���� ���, �� 5000���� ������ �̷���� ��ü�� gpu���� ���Ŀ����� �����Ͽ�
// �ſ� �����ӵ��� ��µǰ� �Ѵ�. 

// �ȼ� ���̴�
// ������ ���� �����ϴ� ���̴� ���α׷���
// ȭ�鿡 �׷��� �� �ȼ����� ���ɿ��� ����
// �پ��� ����Ʈ ȿ�� ���� (�׸���, ��� ��, �ؽ�ó ��)

// HLSL
// ���̴� ���α׷��� ���
// ������ C�� �ſ� ���
// ��������, Ÿ�� ����, ���ؽ�, �ȼ�, ������Ʈ�� ���̴��� �ۼ��� �� ����

using namespace DirectX;

ColorShaderClass::ColorShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
}

ColorShaderClass::~ColorShaderClass()
{

}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	wchar_t vsFileName[128];
	wchar_t psFileName[128];
	int error;

	// �������� hlsl������ ��θ� �����ڵ�� ��ȯ
	error = wcscpy_s(vsFileName, 128, L"../rasterketTutorial/Engine/colorVS.hlsl");
	if (error != 0)
	{
		return false;
	}

	error = wcscpy_s(psFileName, 128, L"../rasterketTutorial/Engine/colorPS.hlsl");
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

void ColorShaderClass::Shutdown()
{
	ShutdownShader();
}

bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount,
	XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	bool result;

	// ������� ����
	result = SetShaderParamters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// �Է� ���̾ƿ�, ���̴� ���� �� Draw Call
	RenderShader(deviceContext, indexCount);

	return true;
}

// �Է� ���̾ƿ�, ��� ����, ���̴� �������̽��� ����
bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	ID3D10Blob* errorMessage = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2] = {};
	unsigned int numElements;
	D3D11_BUFFER_DESC MatrixBufferDesc = {};

	// ���̴� �ڵ� ��ȿ�� ������
	result = D3DCompileFromFile(
		vsFilename,                     // ���̴� �ڵ尡 ���Ե� ���� �̸�
		NULL, NULL,                     // ��
		"ColorVertexShader",            // ���̴� �ڵ� ������
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
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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
	polygonLayout[0].SemanticName         = "POSITION";                  // gpu���� �ν��� ���� �ø�ƽ 
	polygonLayout[0].SemanticIndex        = 0;                           // ���� �̸��� �ø�ƽ�� ��쿡 ���, �� ������ ����
	polygonLayout[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT; // ���� ���� ����, 12byte floatŸ�� ���
	polygonLayout[0].InputSlot            = 0; 
	polygonLayout[0].AlignedByteOffset    = 0;                           // ������ ������, gpu�� �ö� �����ʹ� �Ѳ����� ���޵Ǳ� ������ ���� ���� offset�� �˷������
	polygonLayout[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA; // ���ؽ��� ���, �ٸ� ���� �ν��Ͻ̿� ���
	polygonLayout[0].InstanceDataStepRate = 0;


	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT; 
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

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

	return true;
}

void ColorShaderClass::ShutdownShader()
{
	if(m_vertexShader) 
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
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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
bool ColorShaderClass::SetShaderParamters(ID3D11DeviceContext* deviceContext, 
	DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// d3d11 ���Ŀ� �´� ��Ʈ���� �������� ��ȯ
	worldMatrix      = XMMatrixTranspose(worldMatrix);
	viewMatrix       = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

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
	dataPtr->world      = worldMatrix;
	dataPtr->view       = viewMatrix;
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

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// �Է� ���̾ƿ� ����
	deviceContext->IASetInputLayout(m_layout);

	// ���ؽ� �� �ȼ� ���̴� ����
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// ������ �ε��� ����ŭ �׸���.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}