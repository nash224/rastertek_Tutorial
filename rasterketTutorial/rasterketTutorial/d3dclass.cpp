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
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D_FEATURE_LEVEL featurelevel = {};
	ID3D11Texture2D* backBufferPtr = nullptr;
	float fileOfView = 0.0f;
	float screenAspect = 0.0f;

	numerator = denominator = 0;


	m_vsync_enabled = vsync;

	// DirectX �׷��� �������̽��� ��ü �̸����� �����´�.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// �׷��� �������̽��� �ټ��� ��͸� ������ �� �ִ�.
	// ��� �������̽��� �ϵ������ ������ ������ �� �ִ�.
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// �ش� ��Ϳ� ����� ����� �ϵ���� ������ �����´�.
	result = adapter->EnumOutputs(0, &adapteroutput);
	if (FAILED(result))
	{
		return false;
	}

	// ��û�� �䱸���װ� ��ġ�ϴ� ������� ���� �����´�.
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

	// �䱸���װ� ��ġ�ϴ� ������� ������ �����´�.
	result = adapteroutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}
	
	// ��������ȭ�� �ʿ��� ����� �ֻ����� �����´�.
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

	// ��� ������ �����´�.
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// mega byte ������ �׷���ī�� �޸�
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// �׷���ī���� �ν� �ڵ带 �����´�.
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

	// ����ü���� �������� ���۸� ���� ��ü�Ͽ� 
	// �������� ������� �ո鿡 ��� �� �ֵ��� �������ִ� �༮�̴�.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1; // buffercount�� 1�� ������ ���, ���� ���۸� ����
	swapChainDesc.BufferDesc.Width  = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ �ȼ� Ÿ��

	if (m_vsync_enabled)
	{
		// ���� ����ȭ�� ���� ����� �ֻ��� ��
		swapChainDesc.BufferDesc.RefreshRate.Numerator   = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ������ ��� �뵵
	swapChainDesc.OutputWindow = hwnd; // �������� ������ �ڵ�

	// ��Ƽ ���ø� 
	// �ȼ����� ��������� ��ȭ�ϱ����� ������ ���
	swapChainDesc.SampleDesc.Count   = 1; // �ȼ� �� ������ ǥ�� ����
	swapChainDesc.SampleDesc.Quality = 0; // ǰ�� ����

	// ��üȭ�� ���
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

	// ���۸� ��ȯ ��, ����ۿ� ���� ó��
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰� ������ �⺻������ ����
	swapChainDesc.Flags = 0;

	// gpu�� ��ɼ����� 11�� ����
	featurelevel = D3D_FEATURE_LEVEL_11_0;

	// ����ü�� �ۼ��� ������� d3d ����̽�, ����̽� ���ؽ�Ʈ, ����ü���� �����Ѵ�.
	// ���̷��� 11������ �������� �ʴ� �׷���ī���� ������ ���̴�.
	// 
	// ���� ����Ʈ���� �󿡼� �����ϵ��� �����ϰ� �ʹٸ�, 
	// �ι� ° �Ű����� drivertype�� HARDWARE���� REFERENCE�� �����ϸ� �ȴ�.
	// REFERENCE Ÿ���� gpu�� �ƴ� cpu���� �������Ǵ� ����� �����Ѵ�.
	// 
	// �׸��� ������ ���� ������ ����̽��� ������ ���, 
	// gpu�� �������� cpu�� ����Ʈ ������ �νĵǾ� �����ϴ� ��쵵 �ִ�.
	// �̷��� ������ ���Ǹ� ���� ã�� �׷���ī��� �����ϸ� �ذ�ȴ�.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featurelevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

	// ����ü�� ���ο� �ִ� ������ �����͸� �����´�.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// ����Ÿ�� ��(RenderTargetView, RTV)�� ���۸� ������ �� ������, ������ ������� �νĵ� �� �ִ�.
	// GPU�� �ڿ��� ����� ���, ���۸� ���� ��ġ�� �ʰ� view ��ü�� ���Ͽ� ���� �� ����Ѵ�.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// ������ ���۷��� ī���͸� ���δ�.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// ���̹��۴� 3D�������� ��ü�� ���̸� ǥ���ϱ� ���� �ؽ�ó
	// ����ۿ� ����������, ���̹��۴� �ȼ����� ������ �� �ְ� View��ü�� �ʿ��ϴ�.
	// 
	// ���̹��۴� ����ۿ� �پ� ������, ��쿡 ���� ���Ľ��� ������ �� �ִ�.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width  = screenWidth;
	depthBufferDesc.Height = screenHeight;
	// �Ÿ��� ���� �̸� ����� �ٿ��� �ؽ�ó�� ����
	// LOD����� �����Ǿ� �ִ�.
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	
	depthBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT; // ���̿� ���Ľ� ������ �ȼ��� ũ�⸦ ���� 24����Ʈ, 8����Ʈ�� ���
	depthBufferDesc.SampleDesc.Count   = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL; // ���������ο� ������ ����� �� �ΰ�
	depthBufferDesc.CPUAccessFlags     = 0; // cpu ������� x
	depthBufferDesc.MiscFlags          = 0;

	// ���̹��� �ؽ�ó ����
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// ���Ľ��� ��� ������ ȿ���� ���� ���� �ؽ�ó�� �ȼ��� �ο��ϴ� �ؽ�ó��.
	// ���̹��ۿ� �Բ� �����Ǹ�, ���ٽ��� ��� ����ؾ��ϴ��� ��� ���ձ����� �˷���� �Ѵ�.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true; // ���� Ȱ��ȭ
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable    = true; // ���ٽ� Ȱ��ȭ
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

	// ��� ���ձ⿡ ���ٽ� ������Ʈ ���ε�
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// �ռ� ���ߵ���, ���ٽ� ���� ���̹���ó�� �ؽ�ó�� ��޹޴´�.
	// �ؽ�ó�� �����ϱ����� RTV�� �ʿ��ϵ�, �ؽ�ó�� View ��ü�� �䱸�Ѵ�.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// DSV ����
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	// GPU�� �ڿ��� ����� �� �ֵ��� ��� ���ձ⿡ ����� view�� depth�� view�� �ǳ��ش�.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// �ȼ� ���̴��� �Ѿ�� ���� �����Ͷ������� ������ ���� �ȼ��� ���������Ѵ�.
	// ������ ��� ���������� �� ��� ���ձ⿡ �˷���� �Ѵ�.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK; // �ĸ��� �׸��� ����
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true; // ���� Ŭ���� ����
	rasterDesc.FillMode = D3D11_FILL_SOLID; // ������ �׷���
	rasterDesc.FrontCounterClockwise = false; // CCW, �ð� �ݴ������ �ĸ�
	rasterDesc.MultisampleEnable = false; // ��Ƽ ���ø� ����
	rasterDesc.ScissorEnable = false; // ���� �׽�Ʈ x
	rasterDesc.SlopeScaledDepthBias = 0.f; 

	// �����Ͷ����� state ����
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	// �����Ͷ����� ����
	m_deviceContext->RSSetState(m_rasterState);

	// ����Ʈ�� NDC��ǥ�踦 ȭ�� ��ǥ��� �����ϱ� ���� ���ȴ�.
	m_viewport.Width  = (float)screenWidth;
	m_viewport.Height = (float)screenHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	// ����Ʈ ����
	m_deviceContext->RSSetViewports(1, &m_viewport);

	fileOfView = DirectX::XM_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// ���� ����� 3������ ������ 2������ ��Ƴ��� ���� ����̴�.
	// 2���� ����ü�� �����ϴ� ���̴� �ܰ迡�� ���� ���̴�.
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fileOfView, screenAspect, screenNear, screenDepth);

	// ���� ����� 3���� �ٸ� ��ǥ�� ��ü�� ��ġ�ϱ� ���� ���� ������ ������ ����̴�.
	// �Ϲ������� ��������� ����ϸ�, ũ��, ȸ��, �̵������ ���յ� �����̴�.
	// ���̴� �ܰ迡�� ���Ǳ� ������ �̸� �����Ͽ�, �ʿ� �ÿ� ���̴��� ����� ���޵� ���̴�
	m_WorldMatrix = DirectX::XMMatrixIdentity();

	// ���������� 3���� ������ ī�޶� �������� �����Ͽ� 2������ ����� ��Ƴ��� ����̴�. 
	// �Ϲ������� 2D�� ������ �� ���� �� �ִ�.
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void D3DClass::Shutdown()
{
	// ��ü���� ����ü���� �����ϸ� ���ܰ� �߻��� ���̴�.
	// ����ü���� ��ȯ�ϱ� ���� â���� ��ȯ�ؾ� �Ѵ�.
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
	
	// �Ʒ��� ���� view ��ü�� �ڿ��� �����Ѵ�.

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
		// �ٷ� ���
		m_swapChain->Present(0, 0);
	}
}
