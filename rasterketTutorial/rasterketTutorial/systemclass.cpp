//***************************************************************************************
// systemclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "systemclass.h"

#include <string>

SystemClass::SystemClass()
{
	// ������ �Ҹ��� ���� ��� �����͸� nullptr�� �ʱ�ȭ
	// ��ȿ ��ü�� �ƴ� ���, �߸��� ���� ����
	m_applicationName = 0;
	m_hinstance = 0;
	m_hwnd = 0;

	m_Input = 0;
}

SystemClass::~SystemClass()
{

}

bool SystemClass::Initialize()
{
	// 1. ������ �ʱ�ȭ
	// 2-1. �Է� ��ü ���� �� �ʱ�ȭ
	// 2-2. ���ø����̼� ��ü ���� �� �ʱ�ȭ

	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	// ������ �ʱ�ȭ
	InitializeWindows(screenWidth, screenHeight);

	// �Է� ��ü ���� �� �ʱ�ȭ
	m_Input = new InputClass;
	m_Input->Initialize();

	// ���� ���� ��ü ���� �� �ʱ�ȭ
	// ApplictaionClass
	m_Application = new ApplicationClass;
	result = m_Application->Initialize(screenWidth, screenHeight, m_hwnd);

	return result;
}

void SystemClass::Shutdown()
{
	// ���ø����̼� ��ü ����
	if (m_Application)
	{
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}
	
	// �Է� ��ü ����
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// ����ȭ�� ���� �� �ڵ� ��ȯ
	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;

	// �⺻ �޽��� �����Դϴ�:
	while (!done)
	{
		// �޼����� �ִٸ� �޼����� �����´�.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// �޼����� ���ڷ� ��ȯ
			TranslateMessage(&msg);
			// ������ ���ν����� �޼����� ����
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// ��¿� ������ �߻��� ��� ������ ���´�.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
	
	return;
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
	case WM_KEYDOWN:
	{
		m_Input->KeyDown((unsigned int) wparam);
		static std::wstring output;
		output.clear();

		std::string input = std::to_string((unsigned int)wparam);
		output.assign(input.begin(), input.end());
		output += L"Key has been pressed.\n";
		OutputDebugString(output.c_str());
		return 0;
	}
	case WM_KEYUP:
	{
		m_Input->KeyUp((unsigned int) wparam);
		return 0;
	}
	default:
	{
		// �ٸ� �޼������� ������� �ʰڴٰ� �ý��ۿ� �˸���.
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
	return LRESULT();
}

bool SystemClass::Frame()
{
	bool result = true;

	// �Է�ó��
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// ���ø����̼� ���� ���� ����
	result = m_Application->Frame();
	if (!result)
	{
		return false;
	}

	return result;
}

void SystemClass::InitializeWindows(int& _ScreenWidth, int& _ScreenHeight)
{
	WNDCLASSEXW wcex;
	DEVMODE dmScreenSettings; // ������ ȭ�� �ʱ�ȭ �� ȯ������ ����ü
	int posX, posY;

	// �ý��� ���� ������
	ApplicationHandle = this;

	// �ڵ� ����
	m_hinstance = GetModuleHandle(NULL);

	// ���ø����̼� �̸� �ʱ�ȭ
	m_applicationName = L"Engine";

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hinstance;
	wcex.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_applicationName;
	wcex.hIconSm = wcex.hIcon;

	// ������ ���
	RegisterClassEx(&wcex);

	_ScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
	_ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// ��üȭ�� ����� ��� ��üȭ�� ũ��� ����
	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings); // ����ü ���� �ε�
		dmScreenSettings.dmPelsWidth  = sizeof(dmScreenSettings); // ȭ�� ���̸�ŭ �ȼ� ���
		dmScreenSettings.dmPelsHeight = sizeof(dmScreenSettings); // ȭ�� �ʺ�ŭ �ȼ� ���
		dmScreenSettings.dmBitsPerPel = 32;							// ȭ�� �ȼ��� ��Ʈ ����
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // � ������ �ʱ�ȭ�ƴ��� �˷���� �Ѵ�.

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// �»���� 0���� ����
		posX = posY = 0;
	}
	else
	{
		_ScreenWidth = 800;
		_ScreenHeight = 600;

		// ȭ���� �߽ɿ� �ε��� �������� �ý��� ȭ������ �߷��Ѵ�.
		posX = (GetSystemMetrics(SM_CXSCREEN - _ScreenWidth)) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN - _ScreenHeight)) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, _ScreenWidth, _ScreenHeight, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd); // ȭ�� ���ο��� ���콺�� �νĵǵ��� ����
	SetFocus(m_hwnd); // ���� ȭ���� ��쿡�� Ű���尡 �νĵǵ��� ����

	// ������ Ŀ���� ������ �ʵ��� ����
	ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// ������ ���� �� �ڵ� ��ȯ
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	ApplicationHandle = NULL;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		// �ý��ۿ� �����尡 ����Ȱ� �˸���.
		PostQuitMessage(0);
		return 0;
	}
	break;
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		// �ż����� ���� �Է� ó��
		return ApplicationHandle->MessageHandler(hWnd, message, wParam, lParam);
	}
	}

	return 0;
}
