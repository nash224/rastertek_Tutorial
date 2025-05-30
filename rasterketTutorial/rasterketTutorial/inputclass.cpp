//***************************************************************************************
// inputclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "inputclass.h"

InputClass::InputClass()
	:m_keyboardState{}
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
	m_mouseState = {};
	m_screenWidth = 0;
	m_screenHeight = 0;
	m_mouseX = 0;
	m_mouseY = 0;
}

InputClass::~InputClass()
{

}

bool InputClass::IsEscapePressed() const
{
	return (m_keyboardState[DIK_ESCAPE] & 0x80);
}

void InputClass::GetMouseLocation(int& mouseX, int& mouseY) const
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

bool InputClass::IsMousePressed() const
{
	return (m_mouseState.rgbButtons[0] & 0x80);
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_mouseX = 0;
	m_mouseY = 0;

	// 버전에 맞는 DxInput 인터페이스 생성
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// GUID: 디바이스 식별 고유 코드
	// 키보드 상호작용 인터페이스 생성
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// window focus 소유권을 얻기 전에 키보드를 어떻게 사용할건지 알려줘야 한다.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// 키보드에 대한 접근권한을 얻어온다.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// GUID: 디바이스 식별 고유 코드
	// 마우스 상호작용 인터페이스 생성
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// window focus 소유권을 얻기 전에 마우스 타입을 알려줘야 한다.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// 마우스는 focus에 대한 소유권을 다른 프로그램과 공유하지 않음
	// @TODO: 마우스가 메인화면에서 벗어났을 경우, 어떤 현상이 일어나는지 확인
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// 마우스를 조작할 권한을 얻어온다.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::Shutdown()
{
	if (m_mouse)
	{
		// 소유권 반환
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}
	if (m_keyboard)
	{
		// 소유권 반환
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool InputClass::Frame()
{
	bool result;

	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;

	// 키보드의 현재 상태를 가져온다.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// 키보드 처리 소유권을 뺏기거나 조작 권한을 얻지 못할 경우, 다시 소유권 획득
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}
		 
	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	// 키보드의 현재 상태를 가져온다.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// 키보드 처리 소유권을 뺏기거나 조작 권한을 얻지 못할 경우, 다시 소유권 획득
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

// 마우스의 이동량으로 현재 위치를 갱신한다.
void InputClass::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// 마우스가 화면 밖으로 나갈 경우, 모서리 부분에 위치시킨다.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}