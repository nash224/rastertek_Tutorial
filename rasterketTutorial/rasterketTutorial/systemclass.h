//***************************************************************************************
// systemclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_SYSTEMCLASS_H_
#define __DEFINE_SYSTEMCLASS_H_


//---------------------------------------------------------------------------------------
// PRE-PROCESSING DIRECTIVES
//---------------------------------------------------------------------------------------

// 목적: 사용하지 않는 win32 API일부 기능을 전처리 구문에서 제외하겠다는 선언
// 이유: 기본 구성을 x64 버전으로 사용하기 때문에 win32 기능이 거의 필요없다.
//		내부에 아래 전처리 선언이 되어있다면, 
//		일부 함수가 컴파일되지 않도록 선언되어 있을 것이다.

#define WIN32_LEAN_AND_MEAN

//---------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------

#include <windows.h>

#include "applicationclass.h"
#include "inputclass.h"

//---------------------------------------------------------------------------------------
// Class name: SystemClass
//---------------------------------------------------------------------------------------

// 목적: 응용 프로그램을 동작시키기 위한 클래스
// 기능: 
//		1. 응용 프로그램 실행
//		2. 응용 프로그램 입력 처리
//		3. platform에 필요한 객체 관리

class SystemClass
{
public:
	// constructor & destructor
	SystemClass();
	~SystemClass();

	// @brief		Initialize: 윈도우와 입력, 어플리케이션 객체를 초기화시킨다.
	// 
	// @return		bool	초기화 성공 유무
	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int& _ScreenWidth, int& _ScreenHeight);

	/**
	 * @brief		ShutdownWindows		윈도우를 끄는 작업을 수행한다.
	 *
	 * @return		void
	 *
	 * @warning		null
	 */
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input; // 입력 관리자
	ApplicationClass* m_Application; // 게임 루프 관리

};


//---------------------------------------------------------------------------------------
// PROTOTYPE
//---------------------------------------------------------------------------------------

/**
 * @brief		WndProc		윈도우 메세지를 받아 입력 처리
 *
 * @param		hWnd		윈도우에 대한 핸들
 * @param		message		메세지
 * @param		wParam		메세지 추가 정보
 * @param		lParam		메세지 추가 정보
 *
 * @return		void
 */
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


//---------------------------------------------------------------------------------------
// GLOBALS
//---------------------------------------------------------------------------------------
static SystemClass* ApplicationHandle = 0;


#endif // !__DEFINE_SYSTEMCLASS_H_
