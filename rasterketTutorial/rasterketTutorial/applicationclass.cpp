//***************************************************************************************
// apllicationclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "applicationclass.h"

// ������: 6���� �� �������� ������ �ø��ų� ���� �� ���� 
// ȸ��:   x,y,z �� ���� ������ ������ ȸ���� �� ����
// �̵�:   x,y,z �̵����ͷ� ������ �̵���ų �� ����
// 
// ������, ȸ��, �̵��� �� ��ĸ��� ǥ�� �����ϰ�, 
// �� ���� ����� ������� ���� ����� transform(���� SRT ���)�̶�� Ī��
// �׸��� ������ ������ (0,0,0)�� ������ �ΰ� ���ϴ� ��ǥ�� ��ġ��Ŵ
//
// ���� cpu �ܰ迡�� ������, ȸ��, �̵� ����� ���� transform�� gpu�� �Ѱ��ְ� ��
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

	// ī�޶� ��ü ����
	m_Camera = new CameraClass;
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// ��Ʈ �ʱ�ȭ
	m_Font = new FontClass;
	result = m_Font->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), 0);
	if (!result)
	{
		return false;
	}

	// FontShader �ʱ�ȭ
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

	// �ؽ�Ʈ �ʱ�ȭ
	m_FpsString = new TextClass;
	result = m_FpsString->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, MAX_TEXT_LENGTH, m_Font, testString1, 10, 10, 0.0f, 1.0f, 0.0f);
	if (!result)
	{
		return false;
	}

	// �ؽ�Ʈ �ʱ�ȭ
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

	// ���콺�� ��ġ�� ���޹޴´�.
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

	// ȭ�鿡 ���𰡸� �׸���.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::Render()
{
	// 3D�� �����ϴ� ������Ʈ�� ��ũ�� ������ ����ϱ� ���� �������� ���
	DirectX::XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// �� ��� ������Ʈ
	m_Camera->Render();

	// WVP �ʱ�ȭ
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	// 2D ������������ ���̸� ����.
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

	// ���� Draw
	result = m_FontShader->Render(m_Direct3D->GetDeviceContext(), m_FpsString->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, m_Font->GetTexture(), m_FpsString->GetPixelColor());
	if (!result)
	{
		return false;
	}

	// 2D�������� �����ٸ� �ٽ� ���̸� �����Ѵ�.
	m_Direct3D->TurnZBufferOn();
	m_Direct3D->DisableAlphaBlending();

	// ȭ�鿡 ���
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

	// �������� ǥ�õǴ� ������
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

// ȭ�鿡 ����� ���콺 ���� ������ �����Ѵ�.
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
