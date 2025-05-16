//***************************************************************************************
// apllicationclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
}

ApplicationClass::~ApplicationClass()
{

}

bool ApplicationClass::Initialize(int screenWidth, int screenHieght, HWND hwnd)
{
	bool result;
	char textureFilename[128];

	m_Direct3D = new D3DClass;
	result = m_Direct3D->Initialize(screenWidth, screenHieght, VSYNC_ENABLED, hwnd, 
									FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// 카메라 객체 생성
	m_Camera = new CameraClass;
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Model 객체 생성
	m_Model = new ModelClass;
	strcpy_s(textureFilename, "../rasterketTutorial/Engine/data/stone01.tga");
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Model", L"Error", MB_OK);
		return false;
	}

	// ColorShader 생성
	m_LightShader = new LightShaderClass;
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize ColorShader", L"Error", MB_OK);
		return false;
	}

	m_Light = new LightClass;
	m_Light->SetDiffuseColor(0.85f, 0.12f, 0.32f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void ApplicationClass::Shutdown()
{
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
}

bool ApplicationClass::Frame()
{
	static float rotation = 0.0f;
	bool result;

	rotation -= DirectX::XMConvertToRadians(DirectX::XM_2PI) * 0.3f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::Render(float rotation)
{
	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰 행렬 업데이트
	m_Camera->Render();

	// WVP 초기화
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = DirectX::XMMatrixRotationX(rotation);

	// 기하 도형 입력 세팅
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// 상수버퍼, 버텍스 쉐이더, 픽셀쉐이더 세팅, Draw Call
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), 
		m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}

	// 화면에 출력
	m_Direct3D->EndScene();

	return true;
}
