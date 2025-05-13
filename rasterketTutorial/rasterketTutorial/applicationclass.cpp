//***************************************************************************************
// apllicationclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "applicationclass.h"

ApplicationClass::ApplicationClass()
{
	m_Direct3D = 0;
}

ApplicationClass::~ApplicationClass()
{

}

bool ApplicationClass::Initialize(int screenWidth, int screenHieght, HWND hwnd)
{
	bool result;

	m_Direct3D = new D3DClass;
	result = m_Direct3D->Initialize(screenWidth, screenHieght, VSYNC_ENABLED, hwnd, 
									FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
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
}

bool ApplicationClass::Frame()
{
	bool result;

	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool ApplicationClass::Render()
{
	m_Direct3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_Direct3D->EndScene();

	return true;
}
