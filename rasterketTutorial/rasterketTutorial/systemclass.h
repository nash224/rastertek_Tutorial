//***************************************************************************************
// systemclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_SYSTEMCLASS_H_
#define __DEFINE_SYSTEMCLASS_H_


//---------------------------------------------------------------------------------------
// PRE-PROCESSING DIRECTIVES
//---------------------------------------------------------------------------------------

// ����: ������� �ʴ� win32 API�Ϻ� ����� ��ó�� �������� �����ϰڴٴ� ����
// ����: �⺻ ������ x64 �������� ����ϱ� ������ win32 ����� ���� �ʿ����.
//		���ο� �Ʒ� ��ó�� ������ �Ǿ��ִٸ�, 
//		�Ϻ� �Լ��� �����ϵ��� �ʵ��� ����Ǿ� ���� ���̴�.

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

// ����: ���� ���α׷��� ���۽�Ű�� ���� Ŭ����
// ���: 
//		1. ���� ���α׷� ����
//		2. ���� ���α׷� �Է� ó��
//		3. platform�� �ʿ��� ��ü ����

class SystemClass
{
public:
	// constructor & destructor
	SystemClass();
	~SystemClass();

	// @brief		Initialize: ������� �Է�, ���ø����̼� ��ü�� �ʱ�ȭ��Ų��.
	// 
	// @return		bool	�ʱ�ȭ ���� ����
	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int& _ScreenWidth, int& _ScreenHeight);

	/**
	 * @brief		ShutdownWindows		�����츦 ���� �۾��� �����Ѵ�.
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

	InputClass* m_Input; // �Է� ������
	ApplicationClass* m_Application; // ���� ���� ����

};


//---------------------------------------------------------------------------------------
// PROTOTYPE
//---------------------------------------------------------------------------------------

/**
 * @brief		WndProc		������ �޼����� �޾� �Է� ó��
 *
 * @param		hWnd		�����쿡 ���� �ڵ�
 * @param		message		�޼���
 * @param		wParam		�޼��� �߰� ����
 * @param		lParam		�޼��� �߰� ����
 *
 * @return		void
 */
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


//---------------------------------------------------------------------------------------
// GLOBALS
//---------------------------------------------------------------------------------------
static SystemClass* ApplicationHandle = 0;


#endif // !__DEFINE_SYSTEMCLASS_H_
