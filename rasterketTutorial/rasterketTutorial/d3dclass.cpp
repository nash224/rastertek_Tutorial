//***************************************************************************************
// d3dclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "d3dclass.h"

D3DClass::D3DClass()
	: m_vsync_enabled(false)
	, m_videoCardMemory(0)
	, m_videoCardDescription{0}
	, m_swapChain(nullptr)
	, m_device(nullptr)
	, m_deviceContext(nullptr)
	, m_renderTargetView(nullptr)
	, m_depthStencilBuffer(nullptr)
	, m_depthStencilState(nullptr)
	, m_depthDisabledStencilState(nullptr)
	, m_depthStencilView(nullptr)
	, m_rasterState(nullptr)
	, m_projectionMatrix(DirectX::XMMatrixIdentity())
	, m_WorldMatrix(DirectX::XMMatrixIdentity())
	, m_orthoMatrix(DirectX::XMMatrixIdentity())
	, m_viewport{0}
{

}

D3DClass::~D3DClass()
{

}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, 
						bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	IDXGIOutput* adapteroutput = nullptr;
	unsigned int numModes = 0;
	unsigned int i = 0;
	unsigned int numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList = nullptr;
	DXGI_ADAPTER_DESC adapterDesc;
	int error = 0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D_FEATURE_LEVEL featurelevel = {};
	ID3D11Texture2D* backBufferPtr = nullptr;
	float fileOfView = 0.0f;
	float screenAspect = 0.0f;

	numerator = denominator = 0;


	m_vsync_enabled = vsync;

	// DirectX 그래픽 인터페이스를 객체 이름으로 가져온다.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// 그래픽 인터페이스는 다수의 어뎁터를 가져올 수 있다.
	// 어뎁터 인터페이스로 하드웨어의 정보를 가져올 수 있다.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// 해당 어뎁터와 연결된 모니터 하드웨어 정보를 가져온다.
	result = adapter->EnumOutputs(0, &adapteroutput);
	if (FAILED(result))
	{
		return false;
	}

	// 요청한 요구사항과 일치하는 모니터의 수를 가져온다.
	result = adapteroutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 
		DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// 요구사항과 일치하는 모니터의 정보를 가져온다.
	result = adapteroutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}
	
	// 수직동기화에 필요한 모니터 주사율을 가져온다.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenWidth)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// 어뎁터 정보를 가져온다.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// mega byte 단위의 그래픽카드 메모리
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 그래픽카드의 인식 코드를 가져온다.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	delete[] displayModeList;
	displayModeList = 0;

	adapteroutput->Release();
	adapteroutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	// 스왑체인은 렌더링된 버퍼를 서로 교체하여 
	// 렌더링된 결과물을 앞면에 띄울 수 있도록 보장해주는 녀석이다.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1; // buffercount를 1로 설정할 경우, 더블 버퍼링 지원
	swapChainDesc.BufferDesc.Width  = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 픽셀 타입

	if (m_vsync_enabled)
	{
		// 수직 동기화를 위한 모니터 주사율 값
		swapChainDesc.BufferDesc.RefreshRate.Numerator   = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼의 사용 용도
	swapChainDesc.OutputWindow = hwnd; // 렌더링할 윈도우 핸들

	// 멀티 샘플링 
	// 픽셀들의 계단현상을 완화하기위한 렌더링 기법
	swapChainDesc.SampleDesc.Count   = 1; // 픽셀 당 추출할 표본 개수
	swapChainDesc.SampleDesc.Quality = 0; // 품질 레벨

	// 전체화면 모드
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling          = DXGI_MODE_SCALING_UNSPECIFIED;

	// 버퍼를 교환 후, 백버퍼에 대한 처리
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가 설정을 기본값으로 세팅
	swapChainDesc.Flags = 0;

	// gpu의 기능수준을 11로 설정
	featurelevel = D3D_FEATURE_LEVEL_11_0;

	// 구조체에 작성을 기반으로 d3d 디바이스, 디바이스 컨텍스트, 스왑체인을 생성한다.
	// 다이렉터 11버전을 지원하지 않는 그래픽카드라면 실패할 것이다.
	// 
	// 만약 소프트웨어 상에서 지원하도록 설정하고 싶다면, 
	// 두번 째 매개변수 drivertype을 HARDWARE에서 REFERENCE로 변경하면 된다.
	// REFERENCE 타입은 gpu가 아닌 cpu에서 렌더링되는 기능을 지원한다.
	// 
	// 그리고 주의할 점은 렌더링 디바이스를 세팅할 경우, 
	// gpu가 있음에도 cpu가 디폴트 값으로 인식되어 실패하는 경우도 있다.
	// 이러한 문제는 질의를 통해 찾은 그래픽카드로 세팅하면 해결된다.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featurelevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

	// 스왑체인 내부에 있는 버퍼의 포인터를 가져온다.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// 렌터타겟 뷰(RenderTargetView, RTV)로 버퍼를 조작할 수 있으며, 렌더링 대상으로 인식될 수 있다.
	// GPU가 자원을 사용할 경우, 버퍼를 직접 거치지 않고 view 객체를 통하여 접근 및 사용한다.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// 버퍼의 레퍼런스 카운터를 줄인다.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// 깊이버퍼는 3D차원에서 물체의 깊이를 표현하기 위한 텍스처
	// 백버퍼와 마찬가지로, 깊이버퍼는 픽셀마다 조작할 수 있고 View객체도 필요하다.
	// 
	// 깊이버퍼는 백버퍼에 붙어 있으며, 경우에 따라 스탠실을 부착할 수 있다.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width  = screenWidth;
	depthBufferDesc.Height = screenHeight;
	// 거리에 따라 미리 사이즈를 줄여둔 텍스처의 수준
	// LOD기법과 연관되어 있다.
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	
	depthBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT; // 깊이와 스탠실 버퍼의 픽셀당 크기를 각각 24바이트, 8바이트씩 사용
	depthBufferDesc.SampleDesc.Count   = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL; // 파이프라인에 무엇을 사용할 것 인가
	depthBufferDesc.CPUAccessFlags     = 0; // cpu 접근허용 x
	depthBufferDesc.MiscFlags          = 0;

	// 깊이버퍼 텍스처 생성
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// 스탠실은 고급 렌더링 효과를 내기 위해 텍스처에 픽셀을 부여하는 텍스처다.
	// 깊이버퍼와 함께 생성되며, 스텐실을 어떻게 사용해야하는지 출력 병합기한테 알려줘야 한다.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true; // 깊이 활성화
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable    = true; // 스텐실 활성화
	depthStencilDesc.StencilReadMask  = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}


	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// 깊이버퍼를 끄기위한 2번째 깊이-스텐실 스테이트
	// 2D렌더링에서 사용된다.
	depthDisabledStencilDesc.DepthEnable = false; // 깊이 비활성화
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable    = true; // 스텐실 활성화
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// 출력 병합기에 스텐실 스테이트 바인딩
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// 앞서 말했듯이, 스텐실 또한 깊이버퍼처럼 텍스처로 취급받는다.
	// 텍스처를 조작하기위해 RTV가 필요하듯, 텍스처도 View 객체를 요구한다.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// DSV 생성
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// GPU가 자원을 사용할 수 있도록 출력 병합기에 백버퍼 view와 depth의 view를 건내준다.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// 픽셀 쉐이더로 넘어가기 전에 레스터라이저로 도형에 따라 픽셀을 건져내야한다.
	// 도형을 어떻게 건져내야할 지 출력 병합기에 알려줘야 한다.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK; // 후면은 그리지 않음
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true; // 깊이 클리핑 있음
	rasterDesc.FillMode = D3D11_FILL_SOLID; // 면으로 그려냄
	rasterDesc.FrontCounterClockwise = false; // CCW, 시계 반대방향이 후면
	rasterDesc.MultisampleEnable = false; // 멀티 샘플링 유무
	rasterDesc.ScissorEnable = false; // 시저 테스트 x
	rasterDesc.SlopeScaledDepthBias = 0.f; 

	// 레스터라이저 state 생성
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// 레스터라이저 세팅
	m_deviceContext->RSSetState(m_rasterState);

	// 뷰포트는 NDC좌표계를 화면 좌표계로 복원하기 위해 사용된다.
	m_viewport.Width  = (float)screenWidth;
	m_viewport.Height = (float)screenHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	// 뷰포트 세팅
	m_deviceContext->RSSetViewports(1, &m_viewport);

	fileOfView = DirectX::XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// 투영 행렬은 3차원의 공간을 2차원에 담아내기 위한 행렬이다.
	// 2차원 절투체로 투영하는 쉐이더 단계에서 사용될 것이다.
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fileOfView, screenAspect, screenNear, screenDepth);

	// 월드 행렬은 3차원 다른 좌표에 물체를 배치하기 위해 세계 기준을 정의한 행렬이다.
	// 일반적으로 단위행렬을 사용하며, 크기, 회전, 이동행렬이 결합된 형태이다.
	// 쉐이더 단계에서 사용되기 때문에 미리 정의하여, 필요 시에 쉐이더로 복사로 전달될 것이다
	m_WorldMatrix = DirectX::XMMatrixIdentity();

	// 직교투영은 3차원 공간을 카메라에 수직으로 투영하여 2차원에 장면을 담아내는 방법이다. 
	// 일반적으로 2D를 구현할 때 사용될 수 있다.
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void D3DClass::Shutdown()
{
	// 전체모드로 스왑체인을 제거하면 예외가 발생할 것이다.
	// 스왑체인을 반환하기 전에 창모드로 변환해야 한다.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4] = {};

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;
	
	// 아래와 같이 view 객체로 자원을 수정한다.

	// clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// clear the depth-stencil buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void D3DClass::EndScene()
{
	if (m_vsync_enabled)
	{
		// Lock
		m_swapChain->Present(1, 0);
	}
	else
	{
		// 바로 출력
		m_swapChain->Present(0, 0);
	}
}

void D3DClass::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

void D3DClass::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}