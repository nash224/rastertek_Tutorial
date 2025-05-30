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

	// ������ �´� DxInput �������̽� ����
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// GUID: ����̽� �ĺ� ���� �ڵ�
	// Ű���� ��ȣ�ۿ� �������̽� ����
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// window focus �������� ��� ���� Ű���带 ��� ����Ұ��� �˷���� �Ѵ�.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Ű���忡 ���� ���ٱ����� ���´�.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// GUID: ����̽� �ĺ� ���� �ڵ�
	// ���콺 ��ȣ�ۿ� �������̽� ����
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// window focus �������� ��� ���� ���콺 Ÿ���� �˷���� �Ѵ�.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// ���콺�� focus�� ���� �������� �ٸ� ���α׷��� �������� ����
	// @TODO: ���콺�� ����ȭ�鿡�� ����� ���, � ������ �Ͼ���� Ȯ��
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// ���콺�� ������ ������ ���´�.
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
		// ������ ��ȯ
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}
	if (m_keyboard)
	{
		// ������ ��ȯ
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

	// Ű������ ���� ���¸� �����´�.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// Ű���� ó�� �������� ����ų� ���� ������ ���� ���� ���, �ٽ� ������ ȹ��
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

	// Ű������ ���� ���¸� �����´�.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// Ű���� ó�� �������� ����ų� ���� ������ ���� ���� ���, �ٽ� ������ ȹ��
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

// ���콺�� �̵������� ���� ��ġ�� �����Ѵ�.
void InputClass::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// ���콺�� ȭ�� ������ ���� ���, �𼭸� �κп� ��ġ��Ų��.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}