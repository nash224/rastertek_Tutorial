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
	m_Sprite = 0;
	m_Timer = 0;
	m_TextureShader = 0;
}

ApplicationClass::~ApplicationClass()
{

}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	char textureFilename[128];

	strcpy_s(textureFilename, "../rasterketTutorial/Engine/data/sprite_data_01.txt");

	m_Direct3D = new D3DClass;
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd,
									FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// 카메라 객체 생성
	m_Camera = new CameraClass;
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Model 객체 생성
	m_Sprite = new SpriteClass;
	result = m_Sprite->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, textureFilename, 200, 200, 0.5f, 0.5f);
	if (!result)
	{
		return false;
	}

	m_Sprite->SetRenderLocation(0, 0);
	m_Sprite->SetDuration(0.15f);

	// ColorShader 생성
	m_TextureShader = new TextureShaderClass;
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize ColorShader", L"Error", MB_OK);
		return false;
	}

	// ColorShader 생성
	m_Timer = new TimeClass;
	result = m_Timer->Initialize();
	if (!result)
	{
		return false;
	}

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

	if (m_Sprite)
	{
		m_Sprite->Shutdown();
		delete m_Sprite;
		m_Sprite = 0;
	}

	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
}

bool ApplicationClass::Frame()
{
	bool result;

	m_Timer->Frame();
	const float deltaTime = m_Timer->GetTime();

	m_Sprite->Update(deltaTime);

	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::Render()
{
	// 3D상에 존재하는 오브젝트를 스크린 공간에 사상하기 위해 직교투영 사용
	DirectX::XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 뷰 행렬 업데이트
	m_Camera->Render();

	// WVP 초기화
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// 2D 렌더링에서는 깊이를 끈다.
	m_Direct3D->TurnZBufferOff();

	// 기하 도형 입력 세팅
	m_Sprite->Render(m_Direct3D->GetDeviceContext());

	// 상수버퍼, 버텍스 쉐이더, 픽셀쉐이더 세팅, Draw Call
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Sprite->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, m_Sprite->GetTexture());
	if (!result)
	{
		return false;
	}

	// 2D렌더링이 끝났다면 다시 깊이를 지원한다.
	m_Direct3D->TurnZBufferOn();

	// 화면에 출력
	m_Direct3D->EndScene();

	return true;
}
