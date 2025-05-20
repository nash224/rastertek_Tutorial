//***************************************************************************************
// apllicationclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "applicationclass.h"

// 스케일: 6개의 축 방향으로 도형을 늘리거나 줄일 수 있음 
// 회전:   x,y,z 각 축을 돌려서 도형을 회전할 수 있음
// 이동:   x,y,z 이동벡터로 도형을 이동시킬 수 있음
// 
// 스케일, 회전, 이동은 각 행렬마다 표현 가능하고, 
// 세 개의 행렬이 순서대로 곱한 행렬을 transform(약어로 SRT 행렬)이라고 칭함
// 그리고 세계의 기준점 (0,0,0)에 도형을 두고 원하는 좌표에 위치시킴
//
// 보통 cpu 단계에서 스케일, 회전, 이동 행렬을 곱해 transform을 gpu로 넘겨주게 됨
// 

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
	char modelFilename[128];
	char textureFilename[128];

	strcpy_s(modelFilename, "../rasterketTutorial/Engine/data/sphere.txt");
	strcpy_s(textureFilename, "../rasterketTutorial/Engine/data/stone01.tga");

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
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename);
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
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);    // 환경광 세팅
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);    // 난반사광 색상 세팅
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);			 // 직사광 방향 세팅
	m_Light->SetSpecularColor(0.0f, 1.0f, 0.0f, 1.0f);	 // 정반사광 방향 세팅
	m_Light->SetSpecularColor(32.0f);					 // 정반사광 세기 세팅

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

	rotation -= DirectX::XMConvertToRadians(DirectX::XM_2PI) * 0.25f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	DirectX::XMVECTOR Front = DirectX::XMVECTOR{ 0.0f, 0.0f, 1.0f, 0.0f };
	DirectX::XMVECTOR Vec = DirectX::XMVector3TransformNormal(Front, DirectX::XMMatrixRotationY(rotation));
	DirectX::XMFLOAT3 Dir;
	DirectX::XMStoreFloat3(&Dir, Vec);
	m_Light->SetDirection(Dir);

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
	DirectX::XMMATRIX scaleMatrix, rotateMatrix, translateMatrix, srMatrix;
	bool result;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰 행렬 업데이트
	m_Camera->Render();

	// WVP 초기화
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, rotation, 0.0f);

	// 도형을 조작하려면 도형의 월드 SRT행렬을 구해야 한다.
	// 월드 SRT행렬은 scale, rotation, transition 
	// 그리고 세계 기준이 되는 월드 행렬(대부분 단위 행렬)을 곱하여 구할 수 있다.

	scaleMatrix = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	rotateMatrix = DirectX::XMMatrixRotationQuaternion(Quaternion);
	translateMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	srMatrix = DirectX::XMMatrixMultiply(scaleMatrix, rotateMatrix);
	worldMatrix = DirectX::XMMatrixMultiply(srMatrix, translateMatrix);

	// 기하 도형 입력 세팅
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// 상수버퍼, 버텍스 쉐이더, 픽셀쉐이더 세팅, Draw Call
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// 화면에 출력
	m_Direct3D->EndScene();

	return true;
}
