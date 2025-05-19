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

// ������� ����
// 
// 3D �������� ��ü�� ȸ���Ϸ���, ��ü�� ��ġ�� ������ �Ǵ� ���� ȸ���ϰ� ��
// 
// x���� ȸ���ϰ� �ʹٸ� x���� ������ �ǰ�, �ٸ� �� ���� ����� ������ ��ġ�� ��
// 
// �̷��� �� ȸ���� �����Ϸ���, ��ü�� ��ġ�� ���� ��Ŀ� ȸ�� ����� ���ϰ� ��
// ��, x�� ȸ�����, y�� ȸ�����, z�� ȸ������� ������� ���ؼ� ���� ȸ�� ����� ���ϰ� ��
// 
// ������ �׷��� �󿡼� �ִϸ��̼��� �����ϱ� ���� ȸ���� �����ϴ� ���� �������� ����
// 
// �׷��� ȸ�� ��ķκ��� �ٽ� �� ���� ȸ�� ������ ��ȯ�ؾ� ��
// ��ȯ�ϴ� ������ �ս��� ���һӴ��� �̰� �� �����Ӹ��� �ϱ⿣ ���� ����� ��ģ���� ����
// 
// �̷��� �� �࿡ ���� ȸ���� ���Ϸ� ������� ��
// 
// ���Ϸ� ������ �������� ������ ����� ������ ������ �ְ�, ������ ������ �߻��ϰ� ��
// 
// 
// �� ������ ������� �ذ� �����ѵ�, ������� ���������� ������, 
// ȸ�� ��ķκ��� �� ���� ȸ������ ��������� ���� �� �ְ�, ������ ������ �ذ��� �� ����
// 
// ���� ȸ���� 3�����ε�, ���� �ϳ��� �� �Ἥ 3������ ������ �����ϴ� 4������ ���·� ȸ���� �����ϰ� ��
// �׷��� ������� ȸ����� ȸ�������� �����ϰ� �ְ�,
// �̰��� �Ǽ� 4���� ���ͷ� ǥ���� �� ����
// 
// ���������� �࿡ ���� ������ ������� ��ȯ�Ͽ� ���ϰ�
// ��������� ��ü�� ȸ���� �� �ִ� ȸ����ķ� ��ȯ�ؼ� ����ϰ� ��
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

	strcpy_s(modelFilename, "../rasterketTutorial/Engine/data/cube.txt");
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
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

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

	m_Light = new LightClass;
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
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

	rotation -= DirectX::XMConvertToRadians(DirectX::XM_2PI) * 0.2f;
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
	DirectX::XMMATRIX scaleMatrix, rotateMatrix, translateMatrix, srMatrix;
	bool result;

	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// �� ��� ������Ʈ
	m_Camera->Render();

	// WVP �ʱ�ȭ
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	{
		// ������ �����Ϸ��� ������ ���� SRT����� ���ؾ� �Ѵ�.
		// ���� SRT����� scale, rotation, transition 
		// �׸��� ���� ������ �Ǵ� ���� ���(��κ� ���� ���)�� ���Ͽ� ���� �� �ִ�.

		// ���ʹϿ��� ȸ����� ȸ�������� �����ϰ� �ִ�.
		// ���ʹϿ����� ȸ������� ���� �� �ִ�.
		DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotation, rotation, rotation);
		rotateMatrix = DirectX::XMMatrixRotationQuaternion(Quaternion);
		translateMatrix = DirectX::XMMatrixTranslation(-2.0f, 0.0f, 0.0f);

		// rotation, translate�� �ٲ� ���, ������ ���������κ��� ������ �Ͼ��.
		worldMatrix = DirectX::XMMatrixMultiply(rotateMatrix, translateMatrix);

		// ���� ���� �Է� ����
		m_Model->Render(m_Direct3D->GetDeviceContext());

		// �������, ���ؽ� ���̴�, �ȼ����̴� ����, Draw Call
		result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
			worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
			m_Light->GetDirection(), m_Light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}
	}

	{
		// �� �ٸ� ������ SRT�� ������� �������� �����Ѵ�..
		scaleMatrix = DirectX::XMMatrixScaling(1.0f, 0.5f, 0.5f);
		rotateMatrix = DirectX::XMMatrixRotationY(rotation);
		translateMatrix = DirectX::XMMatrixTranslation(2.0f, 0.0f, 0.0f);

		srMatrix = DirectX::XMMatrixMultiply(scaleMatrix, rotateMatrix);
		worldMatrix = DirectX::XMMatrixMultiply(srMatrix, translateMatrix);
		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotateMatrix);

		// ���� ���� �Է� ����
		m_Model->Render(m_Direct3D->GetDeviceContext());

		// �������, ���ؽ� ���̴�, �ȼ����̴� ����, Draw Call
		result = m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(),
			worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
			m_Light->GetDirection(), m_Light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}
	}

	// ȭ�鿡 ���
	m_Direct3D->EndScene();

	return true;
}
