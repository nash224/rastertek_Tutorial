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

// 사원수의 도입
// 
// 3D 공간에서 물체를 회전하려면, 물체가 위치한 기준이 되는 축을 회전하게 됨
// 
// x축을 회전하고 싶다면 x축을 돌리면 되고, 다른 축 또한 비슷한 과정을 거치게 됨
// 
// 이러한 축 회전을 적용하려면, 물체가 위치한 월드 행렬에 회전 행렬을 곱하게 됨
// 즉, x축 회전행렬, y축 회전행렬, z축 회전행렬을 순서대로 곱해서 최종 회전 행렬을 구하게 됨
// 
// 하지만 그래픽 상에서 애니메이션을 적용하기 위해 회전을 보간하는 일이 생각보다 잦음
// 
// 그래서 회전 행렬로부터 다시 각 축의 회전 각으로 변환해야 함
// 변환하는 과정은 손실이 심할뿐더러 이걸 매 프레임마다 하기엔 연산 비용이 미친듯이 높음
// 
// 이러한 세 축에 대한 회전을 오일러 각도라고 함
// 
// 오일러 각도의 문제점은 위에서 언급한 문제를 가지고 있고, 짐벌락 문제가 발생하게 됨
// 
// 
// 위 문제는 사원수로 해결 가능한데, 사원수는 직관적이지 않지만, 
// 회전 행렬로부터 각 축의 회전각을 저비용으로 얻을 수 있고, 짐벌락 문제도 해결할 수 있음
// 
// 축의 회전은 3차원인데, 차원 하나를 더 써서 3차원을 여러개 포함하는 4차원의 형태로 회전을 보관하게 됨
// 그래서 사원수는 회전축과 회전각도를 포함하고 있고,
// 이것을 실수 4개를 벡터로 표현할 수 있음
// 
// 최종적으로 축에 대한 연산은 사원수로 변환하여 구하고
// 사원수에서 물체를 회전할 수 있는 회전행렬로 변환해서 사용하게 됨
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

	// 카메라 객체 생성
	m_Camera = new CameraClass;
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

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

	// 뷰 행렬 업데이트
	m_Camera->Render();

	// WVP 초기화
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	{
		// 도형을 조작하려면 도형의 월드 SRT행렬을 구해야 한다.
		// 월드 SRT행렬은 scale, rotation, transition 
		// 그리고 세계 기준이 되는 월드 행렬(대부분 단위 행렬)을 곱하여 구할 수 있다.

		// 쿼터니온은 회전축과 회전각도를 포함하고 있다.
		// 쿼터니온으로 회전행렬을 구할 수 있다.
		DirectX::XMVECTOR Quaternion = DirectX::XMQuaternionRotationRollPitchYaw(rotation, rotation, rotation);
		rotateMatrix = DirectX::XMMatrixRotationQuaternion(Quaternion);
		translateMatrix = DirectX::XMMatrixTranslation(-2.0f, 0.0f, 0.0f);

		// rotation, translate을 바꿀 경우, 도형의 기준점으로부터 공전이 일어난다.
		worldMatrix = DirectX::XMMatrixMultiply(rotateMatrix, translateMatrix);

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
	}

	{
		// 또 다른 도형을 SRT를 적용시켜 렌더링을 수행한다..
		scaleMatrix = DirectX::XMMatrixScaling(1.0f, 0.5f, 0.5f);
		rotateMatrix = DirectX::XMMatrixRotationY(rotation);
		translateMatrix = DirectX::XMMatrixTranslation(2.0f, 0.0f, 0.0f);

		srMatrix = DirectX::XMMatrixMultiply(scaleMatrix, rotateMatrix);
		worldMatrix = DirectX::XMMatrixMultiply(srMatrix, translateMatrix);
		worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotateMatrix);

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
	}

	// 화면에 출력
	m_Direct3D->EndScene();

	return true;
}
