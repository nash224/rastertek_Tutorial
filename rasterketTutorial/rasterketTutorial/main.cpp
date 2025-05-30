
//---------------------------------------------------------------------------------------
// Filename: main.ccp
//---------------------------------------------------------------------------------------

// �� ������ �������� �ǵ��� �ڵ� ��Ÿ��, ���� ������ 
// ��� �Ǵ� Leak�� �߻����� �ʴ� ������ 
// ö���� �����ϰ� �ۼ��Ǿ����� �˷��帳�ϴ�. 

// �߽�: ������ �ڵ忡�� �ڿ� ������ ������ ���, 
// ���������� �ڿ��� �������� �ʴ� ������ �ֽ��ϴ�.
// �������� �ڵ� ��Ÿ���� �ִ��� ����ϰ� �����ϵ��� goto ���� ��������� �˷��帳�ϴ�.
// ����) ModelClass::InitializeBuffers 

// ���� ����Ʈ: https://www.rastertek.com/dx11win10tut16.html

// ����: DirectX Input API�� Ű����� ���콺 �Է��� ó���ϴ� ����� �˾ƺ� ���̴�.

// ���۹�: 
//		- escape:				exit application
//		- move mouse:			for rendering mouse location.
//		- mouse's first button: for rendering to text what is the current status of the mouse.


#include "systemclass.h"
#include <crtdbg.h>

// window ���� ���α׷��� ������
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SystemClass* System;
	bool result;

	// �ý��� ��ü ����
	System = new SystemClass;

	// �ý��� ��ü �ʱ�ȭ
	result = System->Initialize();
	if (result)
	{
		// �ý��� ���� 
		System->Run();
	}

	// �ý��� �ı�
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}