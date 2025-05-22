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

	// ī�޶� ��ü ����
	m_Camera = new CameraClass;
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Model ��ü ����
	m_Sprite = new SpriteClass;
	result = m_Sprite->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, textureFilename, 200, 200, 0.5f, 0.5f);
	if (!result)
	{
		return false;
	}

	m_Sprite->SetRenderLocation(0, 0);
	m_Sprite->SetDuration(0.15f);

	// ColorShader ����
	m_TextureShader = new TextureShaderClass;
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize ColorShader", L"Error", MB_OK);
		return false;
	}

	// ColorShader ����
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

	// ���� ���� �Է� ����
	m_Sprite->Render(m_Direct3D->GetDeviceContext());

	// �������, ���ؽ� ���̴�, �ȼ����̴� ����, Draw Call
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Sprite->GetIndexCount(),
		worldMatrix, viewMatrix, orthoMatrix, m_Sprite->GetTexture());
	if (!result)
	{
		return false;
	}

	// 2D�������� �����ٸ� �ٽ� ���̸� �����Ѵ�.
	m_Direct3D->TurnZBufferOn();

	// ȭ�鿡 ���
	m_Direct3D->EndScene();

	return true;
}
