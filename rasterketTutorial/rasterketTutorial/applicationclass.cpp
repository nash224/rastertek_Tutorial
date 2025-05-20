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
	m_Model = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_numLights = 0;
}

ApplicationClass::~ApplicationClass()
{

}

bool ApplicationClass::Initialize(int screenWidth, int screenHieght, HWND hwnd)
{
	bool result;
	char modelFilename[128];
	char textureFilename[128];

	strcpy_s(modelFilename, "../rasterketTutorial/Engine/data/plane.txt");
	strcpy_s(textureFilename, "../rasterketTutorial/Engine/data/stone01.tga");

	m_Direct3D = new D3DClass;
	result = m_Direct3D->Initialize(screenWidth, screenHieght, VSYNC_ENABLED, hwnd, 
									FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// ī�޶� ��ü ����
	m_Camera = new CameraClass;
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);
	m_Camera->SetPosition(0.0f, 2.0f, -12.0f);

	// Model ��ü ����
	m_Model = new ModelClass;
	result = m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), modelFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Model", L"Error", MB_OK);
		return false;
	}

	// ColorShader ����
	m_LightShader = new LightShaderClass;
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize ColorShader", L"Error", MB_OK);
		return false;
	}

	m_numLights = 4;

	m_Light = new LightClass[m_numLights];
	m_Light[0].SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f); // red
	m_Light[0].SetPosition(-3.0f, 1.0f, 3.0f);

	m_Light[1].SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f); // green
	m_Light[1].SetPosition(3.0f, 1.0f, 3.0f);

	m_Light[2].SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f); // blue
	m_Light[2].SetPosition(-3.0f, 1.0f, -3.0f);

	m_Light[3].SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f); // white
	m_Light[3].SetPosition(3.0f, 1.0f, -3.0f);

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
		delete[] m_Light;
		m_Light = 0;
	}
}

bool ApplicationClass::Frame()
{
	bool result;

	result = Render(0.0f);
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
	DirectX::XMFLOAT4 diffuseColor[4], lightPosition[4]; // 100% �����Ƽ� �̷��� �ѵ�
	int i;
	bool result;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// �� ��� ������Ʈ
	m_Camera->Render();

	// WVP �ʱ�ȭ
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationRollPitchYaw(0.0f, rotation, 0.0f);

	// ������ �����Ϸ��� ������ ���� SRT����� ���ؾ� �Ѵ�.
	// ���� SRT����� scale, rotation, transition 
	// �׸��� ���� ������ �Ǵ� ���� ���(��κ� ���� ���)�� ���Ͽ� ���� �� �ִ�.

	scaleMatrix = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	rotateMatrix = DirectX::XMMatrixRotationQuaternion(Quaternion);
	translateMatrix = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	srMatrix = DirectX::XMMatrixMultiply(scaleMatrix, rotateMatrix);
	worldMatrix = DirectX::XMMatrixMultiply(srMatrix, translateMatrix);

	for (i = 0; i < m_numLights; i++)
	{
		diffuseColor[i] = m_Light[i].GetDiffuseColor();
		lightPosition[i] = m_Light[i].GetPosition();
	}

	// ���� ���� �Է� ����
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// �������, ���ؽ� ���̴�, �ȼ����̴� ����, Draw Call
	result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
		worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), diffuseColor, lightPosition);
	if (!result)
	{
		return false;
	}

	// ȭ�鿡 ���
	m_Direct3D->EndScene();

	return true;
}
