//***************************************************************************************
// systemclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "systemclass.h"

#include <string>

SystemClass::SystemClass()
{
	// 안전한 소멸을 위해 멤버 포인터를 nullptr로 초기화
	// 유효 객체가 아닐 경우, 잘못된 해제 방지
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
	// 1. 윈도우 초기화
	// 2-1. 입력 객체 생성 및 초기화
	// 2-2. 어플리케이션 객체 생성 및 초기화

	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	// 윈도우 초기화
	InitializeWindows(screenWidth, screenHeight);

	// 입력 객체 생성 및 초기화
	m_Input = new InputClass;
	m_Input->Initialize();

	// 게임 루프 객체 생성 및 초기화
	// ApplictaionClass
	m_Application = new ApplicationClass;
	result = m_Application->Initialize(screenWidth, screenHeight, m_hwnd);

	return result;
}

void SystemClass::Shutdown()
{
	// 어플리케이션 객체 정리
	if (m_Application)
	{
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}
	
	// 입력 객체 정리
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// 메인화면 종료 및 핸들 반환
	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	done = false;

	// 기본 메시지 루프입니다:
	while (!done)
	{
		// 메세지가 있다면 메세지를 가져온다.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 메세지를 문자로 변환
			TranslateMessage(&msg);
			// 윈도우 프로시저로 메세지를 보냄
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// 출력에 문제가 발생할 경우 루프를 끊는다.
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
		// 다른 메세지들은 사용하지 않겠다고 시스템에 알리낟.
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
	return LRESULT();
}

bool SystemClass::Frame()
{
	bool result = true;

	// 입력처리
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// 어플리케이션 게임 로직 루프
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
	DEVMODE dmScreenSettings; // 윈도우 화면 초기화 및 환경정보 구조체
	int posX, posY;

	// 시스템 전역 포인터
	ApplicationHandle = this;

	// 핸들 복사
	m_hinstance = GetModuleHandle(NULL);

	// 어플리케이션 이름 초기화
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

	// 윈도우 등록
	RegisterClassEx(&wcex);

	_ScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
	_ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 전체화면 모드일 경우 전체화면 크기로 세팅
	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings); // 구조체 버전 로드
		dmScreenSettings.dmPelsWidth  = sizeof(dmScreenSettings); // 화면 높이만큼 픽셀 사용
		dmScreenSettings.dmPelsHeight = sizeof(dmScreenSettings); // 화면 너비만큼 픽셀 사용
		dmScreenSettings.dmBitsPerPel = 32;							// 화면 픽셀당 비트 지정
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // 어떤 변수가 초기화됐는지 알려줘야 한다.

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 좌상단을 0으로 지정
		posX = posY = 0;
	}
	else
	{
		_ScreenWidth = 800;
		_ScreenHeight = 600;

		// 화면을 중심에 두도록 시작점을 시스템 화면으로 추론한다.
		posX = (GetSystemMetrics(SM_CXSCREEN - _ScreenWidth)) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN - _ScreenHeight)) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, _ScreenWidth, _ScreenHeight, NULL, NULL, m_hinstance, NULL);

	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd); // 화면 내부에서 마우스가 인식되도록 설정
	SetFocus(m_hwnd); // 메인 화면일 경우에만 키보드가 인식되도록 설정

	// 윈도우 커서가 보이지 않도록 설정
	ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
	ShowCursor(true);

	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 윈도우 제거 및 핸들 반환
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
		// 시스템에 스레드가 종료된걸 알린다.
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
		// 매세지에 대한 입력 처리
		return ApplicationHandle->MessageHandler(hWnd, message, wParam, lParam);
	}
	}

	return 0;
}
