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
	m_Font = 0;
	m_FontShader = 0;

	m_FpsString = 0;
	m_Fps = 0;
	m_previousFps = 0;
	m_MouseString = 0;
}

ApplicationClass::~ApplicationClass()
{

}

bool ApplicationClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	char textureFilename[128];

	strcpy_s(textureFilename, "../rasterketTutorial/Engine/data/sprite_data_02.txt");

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

	// 폰트 초기화
	m_Font = new FontClass;
	result = m_Font->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 0);
	if (!result)
	{
		return false;
	}

	// FontShader 초기화
	m_FontShader = new FontShaderClass;
	result = m_FontShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize ColorShader", L"Error", MB_OK);
		return false;
	}

	m_Fps = new FpsClass();
	m_Fps->Initialize();

	const int MAX_TEXT_LENGTH = 32;
	char testString1[MAX_TEXT_LENGTH];
	char mouseString1[MAX_TEXT_LENGTH];
	char mouseString2[MAX_TEXT_LENGTH];
	char mouseString3[MAX_TEXT_LENGTH];

	strcpy_s(testString1, "Fps: 0");
	strcpy_s(mouseString1, "Mouse X: 0");
	strcpy_s(mouseString2, "Mouse Y: 0");
	strcpy_s(mouseString3, "Mouse Button: No");

	// 텍스트 초기화
	m_FpsString = new TextClass;
	result = m_FpsString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, MAX_TEXT_LENGTH, m_Font, testString1, 10, 10, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	// 텍스트 초기화
	m_MouseString = new TextClass[3];
	result = m_MouseString[0].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, MAX_TEXT_LENGTH, m_Font, mouseString1, 10, 35, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}
	result = m_MouseString[1].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, MAX_TEXT_LENGTH, m_Font, mouseString2, 10, 60, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}
	result = m_MouseString[2].Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, MAX_TEXT_LENGTH, m_Font, mouseString3, 10, 85, 0.0f, 1.0f, 0.0f);
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

	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	if (m_FpsString)
	{
		m_FpsString->Shutdown();
		delete m_FpsString;
		m_FpsString = 0;
	}

	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	if (m_MouseString)
	{
		m_MouseString[0].Shutdown();
		m_MouseString[1].Shutdown();
		m_MouseString[2].Shutdown();
		delete[] m_MouseString;
		m_MouseString = 0;
	}
}

bool ApplicationClass::Frame(InputClass* Input)
{
	int mouseX, mouseY;
	bool mouseDown;
	bool result;

	if (Input->IsEscapePressed())
	{
		return false;
	}

	// 마우스의 위치를 전달받는다.
	Input->GetMouseLocation(mouseX, mouseY);

	mouseDown = Input->IsMousePressed();

	result = UpdateMouseStrings(mouseX, mouseY, mouseDown);
	if (!result)
	{
		return false;
	}

	result = UpdateFps();
	if (!result)
	{
		return false;
	}

	// 화면에 무언가를 그린다.
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
	m_Direct3D->EnableAlphaBlending();

	for (int i = 0; i < 3; i++)
	{
		m_MouseString[i].Render(m_Direct3D->GetDeviceContext());

		result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_MouseString[i].GetIndexCount(),
			worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture(), m_MouseString[i].GetPixelColor());
		if (!result)
		{
			return false;
		}
	}

	m_FpsString->Render(m_Direct3D->GetDeviceContext());

	// 문장 Draw
	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_FpsString->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture(), m_FpsString->GetPixelColor());
	if (!result)
	{
		return false;
	}

	// 2D렌더링이 끝났다면 다시 깊이를 지원한다.
	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();

	// 화면에 출력
	m_Direct3D->EndScene();

	return true;
}

bool ApplicationClass::UpdateFps()
{
	int fps;
	char tempString[16];
	char finalString[16];
	bool result;

	m_Fps->Frame();
	fps = m_Fps->GetFps();

	if (m_previousFps == fps)
	{
		return true;
	}

	m_previousFps = fps;

	// 프레임율 표시되는 상한점
	if (fps > 99999)
	{
		fps = 99999;
	}

	sprintf_s(tempString, "%d", fps);

	sprintf_s(finalString, "fps: ");
	strcat_s(finalString, tempString);

	float red, green, blue;

	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	else if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}
	else
	{
		red = 1.0f;
		green = .0f;
		blue = .0f;
	}

	result = m_FpsString->UpdateText(m_Direct3D->GetDeviceContext(), m_Font, finalString, 10, 10, red, green, blue);
	if (!result)
	{
		return false;
	}

	return true;
}

// 화면에 출력할 마우스 상태 정보를 갱신한다.
bool ApplicationClass::UpdateMouseStrings(int mouseX, int mouseY, bool mouseDown)
{
	char tempString[16];
	char FinalString[32];
	bool result;

	sprintf_s(tempString, "%d", mouseX);
	strcpy_s(FinalString, "Mouse X: ");
	strcat_s(FinalString, tempString);

	result = m_MouseString[0].UpdateText(m_Direct3D->GetDeviceContext(), m_Font, FinalString, 10, 35, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	sprintf_s(tempString, "%d", mouseY);
	strcpy_s(FinalString, "Mouse Y: ");
	strcat_s(FinalString, tempString);

	result = m_MouseString[1].UpdateText(m_Direct3D->GetDeviceContext(), m_Font, FinalString, 10, 60, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	strcpy_s(FinalString, mouseDown ? "Mouse Button: Yes" : "Mouse Button: No");

	result = m_MouseString[2].UpdateText(m_Direct3D->GetDeviceContext(), m_Font, FinalString, 10, 85, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		return false;
	}

	return true;
}
