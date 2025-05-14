//***************************************************************************************
// colorshaderclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "colorshaderclass.h"

// 버텍스 버퍼
// 컴퓨터에서 표현하는 모든 물체는 거의 삼각형으로 표현된다. 
// 세 점으로 이루어져 있는 삼각형을 그리기 위해서는 
// 도형의 정보인 점의 배열을 버텍스 버퍼에 채워 gpu로 보내야 한다.

// 인덱스 버퍼
// 인덱스 버퍼는 버텍스 버퍼의 정보를 담고 있는 배열이다.
// 도형을 그리기 위해서 필수 정보는 아니지만 점을 매우 빠른 속도로
// 찾을 수 있고, 그리는 순서도 지정할 수 있어 왠만해서는 버텍스 버퍼와
// 함께 gpu로 보내는 구조체이다.

// 버텍스 쉐이더
// 버텍스 쉐이더는 3d 공간에 있는 점을 옮기기 위한 쉐이더 프로그래밍이다.
// 예를 들어, 약 5000개의 점으로 이루어진 물체를 gpu에서 병렬연산을 수행하여
// 매우 빠른속도로 출력되게 한다. 

// 픽셀 쉐이더
// 도형의 색을 결정하는 쉐이더 프로그래밍
// 화면에 그려질 각 픽셀마다 병령연산 수행
// 다양한 이펙트 효과 구현 (그림자, 모션 블러, 텍스처 등)

// HLSL
// 쉐이더 프로그래밍 언어
// 문법은 C와 매우 흡사
// 전역변수, 타입 정의, 버텍스, 픽셀, 지오메트리 쉐이더를 작성할 수 있음

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

	// 컴파일할 hlsl파일의 경로를 유니코드로 변환
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

	// 상수버퍼 세팅
	result = SetShaderParamters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// 입력 레이아웃, 쉐이더 세팅 및 Draw Call
	RenderShader(deviceContext, indexCount);

	return true;
}

// 입력 레이아웃, 상수 버퍼, 쉐이더 인터페이스를 생성
bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;
	ID3D10Blob* errorMessage = nullptr;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2] = {};
	unsigned int numElements;
	D3D11_BUFFER_DESC MatrixBufferDesc = {};

	// 쉐이더 코드 유효성 컴파일
	result = D3DCompileFromFile(
		vsFilename,                     // 쉐이더 코드가 포함된 파일 이름
		NULL, NULL,                     // 모름
		"ColorVertexShader",            // 쉐이더 코드 진입점
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
	result = D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
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
	polygonLayout[0].SemanticName         = "POSITION";                  // gpu에서 인식할 변수 시멘틱 
	polygonLayout[0].SemanticIndex        = 0;                           // 같은 이름의 시멘틱일 경우에 사용, 몇 번인지 지정
	polygonLayout[0].Format               = DXGI_FORMAT_R32G32B32_FLOAT; // 변수 형식 지정, 12byte float타입 사용
	polygonLayout[0].InputSlot            = 0; 
	polygonLayout[0].AlignedByteOffset    = 0;                           // 변수의 시작점, gpu로 올라간 데이터는 한꺼번에 전달되기 때문에 변수 시작 offset을 알려줘야함
	polygonLayout[0].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA; // 버텍스에 사용, 다른 값은 인스턴싱에 사용
	polygonLayout[0].InstanceDataStepRate = 0;


	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT; 
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

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
bool ColorShaderClass::SetShaderParamters(ID3D11DeviceContext* deviceContext, 
	DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// d3d11 형식에 맞는 메트릭스 형식으로 변환
	worldMatrix      = XMMatrixTranspose(worldMatrix);
	viewMatrix       = XMMatrixTranspose(viewMatrix);
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
	dataPtr->world      = worldMatrix;
	dataPtr->view       = viewMatrix;
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

	return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// 입력 레이아웃 세팅
	deviceContext->IASetInputLayout(m_layout);

	// 버텍스 및 픽셀 쉐이더 세팅
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// 도형의 인덱스 수만큼 그린다.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}