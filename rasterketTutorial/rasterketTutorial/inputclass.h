//***************************************************************************************
// inputclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_INPUTCLASS_H_
#define __DEFINE_INPUTCLASS_H_


// dx Input API�� ���� 8�� ����ص� ���� ������ ���� ������ ������ �ٲ� �ʿ�� ����.
// �׷��� ����� ����� �����ϱ� ���� �Է� API ������ �˷���� �Ѵ�.
//---------------------------------------------------------------------------------------
// PRE-PROCESSING DIRECTIVES 
//---------------------------------------------------------------------------------------
#define DIRECTINPUT_VERSION 0x0800


//---------------------------------------------------------------------------------------
// LINKING 
//---------------------------------------------------------------------------------------
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

//---------------------------------------------------------------------------------------
// INCLUDES 
//---------------------------------------------------------------------------------------
#include <dinput.h>

//---------------------------------------------------------------------------------------
// Classname: InputClass
//---------------------------------------------------------------------------------------

/*
����:
	DirectX Input API�� ���콺 �� Ű���� �Է¿� ���� ����� �����Ѵ�.

���:
	- ������ �Է� ó��
	- ���콺 �̵��� ����
	- ���콺 ��ġ ����
	- Ű���� ���� ���� ����
*/

class InputClass
{
public:
	// constructor & destructor
	InputClass();
	~InputClass();

	// delete function
	InputClass(const InputClass& _Other) = delete;
	InputClass(InputClass&& _Other) noexcept = delete;
	InputClass& operator=(const InputClass& _Other) = delete;
	InputClass& operator=(InputClass&& _Other) noexcept = delete;


	bool IsEscapePressed() const;
	void GetMouseLocation(int& mouseX, int& mouseY) const;
	bool IsMousePressed() const;

	// DirectX Input �������̽��� �����ϰ� �ʱ�ȭ�Ѵ�.
	bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	// Ű���� �� ���콺 �Է� ���¸� �����Ѵ�.
	bool Frame();

protected:

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;
	int m_screenWidth;
	int m_screenHeight;
	int m_mouseX;
	int m_mouseY;

};


#endif // !__DEFINE_INPUTCLASS_H_
