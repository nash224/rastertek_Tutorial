//***************************************************************************************
// inputclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_INPUTCLASS_H_
#define __DEFINE_INPUTCLASS_H_


// dx Input API는 버전 8을 사용해도 워낙 성능이 좋기 때문에 버전을 바꿀 필요는 없다.
// 그러나 경고문이 출력을 방지하기 위해 입력 API 버전을 알려줘야 한다.
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
목적:
	DirectX Input API로 마우스 및 키보드 입력에 대한 기능을 제공한다.

기능:
	- 고성능의 입력 처리
	- 마우스 이동량 측정
	- 마우스 위치 갱신
	- 키보드 눌림 상태 갱신
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

	// DirectX Input 인터페이스를 생성하고 초기화한다.
	bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	// 키보드 및 마우스 입력 상태를 갱신한다.
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
