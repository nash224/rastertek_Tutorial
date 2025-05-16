//***************************************************************************************
// LightShaderClass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "LightShaderClass.h"

using namespace DirectX;

LightShaderClass::LightShaderClass()
{
	m_vertexShader = 0;
	m_pixelShader = 0;
	m_layout = 0;
	m_matrixBuffer = 0;
	m_sampleState = 0;
	m_lightBuffer = 0;
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

	// 컴파일할 hlsl파일의 경로를 유니코드로 변환
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
	ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	bool result;

	// 상수버퍼 세팅
	result = SetShaderParamters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, diffuseColor);
	if (!result)
	{
		return false;
	}

	// 입력 레이아웃, 쉐이더 세팅 및 Draw Call
	RenderShader(deviceContext, indexCount);

	return true;
}

// 입력 레이아웃, 상수 버퍼, 쉐이더 인터페이스를 생성
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
	D3D11_BUFFER_DESC lightBufferDesc = {};

	// 쉐이더 코드 유효성 컴파일
	result = D3DCompileFromFile(
		vsFilename,                     // 쉐이더 코드가 포함된 파일 이름
		NULL, NULL,                     // 모름
		"LightVertexShader",            // 쉐이더 코드 진입점
		"vs_5_0",                       // 쉐이더 모델 버전
		D3D10_SHADER_ENABLE_STRICTNESS, // 컴파일 옵션
		0,                              // 컴파일 옵션 2
		&vertexShaderBuffer,            // 데이터 전달용 객체, 쉐이더 객체를 생성할 때 사용
		&errorMessage                   // 쉐이더 에러 메세지
	);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);

			// 필요하지 않은 메세지 버퍼는 정리한다.
			errorMessage->Release();
			errorMessage = 0;
		}
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing ShaderFile.", MB_OK);
		}

		return false;
	}

	// 픽셀 쉐이더 컴파일
	result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);

			// 필요하지 않은 메세지 버퍼는 정리한다.
			errorMessage->Release();
			errorMessage = 0;
		}
		else
		{
			MessageBox(hwnd, psFilename, L"Missing ShaderFile.", MB_OK);
		}

		return false;
	}

	// 버텍스 쉐이더 생성
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	// 픽셀 쉐이더 생성
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// gpu에서 사용할 구조체와 일치하도록 작성한다.
	polygonLayout[0].SemanticName = "POSITION";                  // gpu에서 인식할 변수 시멘틱 
	polygonLayout[0].SemanticIndex = 0;                           // 같은 이름의 시멘틱일 경우에 사용, 몇 번인지 지정
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // 변수 형식 지정, 12byte float타입 사용
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;                           // 변수의 시작점, gpu로 올라간 데이터는 한꺼번에 전달되기 때문에 변수 시작 offset을 알려줘야함
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // 버텍스에 사용, 다른 값은 인스턴싱에 사용
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";	// 텍셀 좌표 시멘틱 사용
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT; // uv는 2차원 좌표를 요구함
	polygonLayout[1].InputSlot = 0; // 슬롯 0번
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";	// 도형의 노말 벡터
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT; // 방향 벡터(x,y,z)로 float3 사용
	polygonLayout[2].InputSlot = 0; // 슬롯 0번
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 입력 레이아웃 생성
	result = device->CreateInputLayout(
		polygonLayout,                          // 입력 데이터 형식의 배열
		numElements,                            // 입력 데이터 형식 수
		vertexShaderBuffer->GetBufferPointer(), // 컴파일된 쉐이더 코드에 대한 포인터
		vertexShaderBuffer->GetBufferSize(),    // 컴파일된 쉐이더 코드에 대한 크기
		&m_layout                               // 전달받을 포인터
	);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// 상수 버퍼 생성
	MatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;             // 런타임에서 사용하겠다.
	MatrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);    // 구조체 크기
	MatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // 상수 버퍼로 사용
	MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu 접근 금지
	MatrixBufferDesc.MiscFlags = 0;
	MatrixBufferDesc.StructureByteStride = 0;                 // 스트럭처드 버퍼에 사용

	result = device->CreateBuffer(&MatrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 텍스처의 픽셀을 조합하는 방법 서술
	// 레스터라이저를 거친 기하 도형의 픽셀을 
	// 샘플러가 텍스처의 픽셀을 조합하여 알려준다.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// 텍스처를 샘플링할 때 필터링 방법
															// D3D11_FILTER_MIN_MAG_MIP_LINEAR을 선택할 경우, 
															// 샘플러는 선형보간으로 색상을 얻어낸다.

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	// D3D11_TEXTURE_ADDRESS_WRAP:   텍스처 좌표[0,1]을 넘길 경우, uv 반복
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;	// D3D11_TEXTURE_ADDRESS_MIRROR: 텍스처 좌표[0,1]을 넘길 경우, uv 반전
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;  // D3D11_TEXTURE_ADDRESS_CLAMP:  텍스처 좌표를 [0,1]로 clamp 처리
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1; // 이방성 샘플에서 사용될 샘플의 수
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0.0f; // 테두리 색
	samplerDesc.BorderColor[1] = 0.0f; // 테두리 색
	samplerDesc.BorderColor[2] = 0.0f; // 테두리 색
	samplerDesc.BorderColor[3] = 0.0f; // 테두리 색
	samplerDesc.MinLOD = 0;  // mipmap 수준, 가장 높은 품질
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; // 가장 낮은 품질

	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	// 픽셀 쉐이더에서 사용될 light 상수 버퍼
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;             // cpu에서 쓰기 가능
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);     // 구조체 크기
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // 상수 버퍼로 사용
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // cpu에서 쓰기 가능
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;                 // 스트럭처드 버퍼에 사용

	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
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

	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}
}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();

	// 컴파일 실패 메세지를 기록한다.
	fout.open("shader-error.txt");

	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	MessageBox(hwnd, L"Error compile Shader. Check Shader-error.txt for message.", shaderFilename, MB_OK);
}

// 쉐이더 코드에서 사용할 상수 버퍼 세팅
// 픽셀 쉐이더의 전역 텍스처 슬롯에 사용할 텍스처 세팅
bool LightShaderClass::SetShaderParamters(ID3D11DeviceContext* deviceContext,
	DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* dataPtr2;
	unsigned int bufferNumber;

	// d3d11 형식에 맞는 메트릭스 형식으로 변환
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// cpu에서 자원을 gpu로 보낼 때, gpu가 접근하면 문제가 발생한다.
	// gpu가 해당 자원을 접근하지 못하도록 Lock을 건다
	result = deviceContext->Map(
		m_matrixBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD, // cpu에서 쓰고 기존에 gpu에 적제되어 있던 데이터는 모두 덮어 씌움
		0,
		&mappedResource
	);
	if (FAILED(result))
	{
		return false;
	}

	// 여기에 보낼 행렬 데이터를 적제한다.
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// 데이터를 전부 적제했다면 Unlock 수행
	// 그냥 빠져 나갈 시, gpu는 Deadlock
	deviceContext->Unmap(m_matrixBuffer, 0);

	bufferNumber = 0;

	// 상수 버퍼 세팅
	// 상수 버퍼 14개까지 등록 가능
	deviceContext->VSSetConstantBuffers(
		bufferNumber,   // 0번 슬롯 사용
		1,              // 상수버퍼 1개 사용
		&m_matrixBuffer // 버퍼 인터페이스
	);

	// 0번 슬롯에 texture 세팅
	deviceContext->PSSetShaderResources(0, 1, &texture);


	result = deviceContext->Map(
		m_lightBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 
		0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 여기에 보낼 행렬 데이터를 적제한다.
	dataPtr2 = (LightBufferType*)mappedResource.pData;
	dataPtr2->diffuseColor = diffuseColor;
	dataPtr2->lightDirection = lightDirection;
	dataPtr2->padding = 0.0f;

	// 데이터를 전부 적제했다면 Unlock 수행
	// 그냥 빠져 나갈 시, gpu는 Deadlock
	deviceContext->Unmap(m_lightBuffer, 0);

	// 상수 버퍼 세팅
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

	return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// 입력 레이아웃 세팅
	deviceContext->IASetInputLayout(m_layout);

	// 버텍스 및 픽셀 쉐이더 세팅
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// 픽셀 쉐이더 샘플러 0번 슬롯에 샘플러 세팅
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// 도형의 인덱스 수만큼 그린다.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}