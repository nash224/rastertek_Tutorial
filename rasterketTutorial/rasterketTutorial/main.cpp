
//---------------------------------------------------------------------------------------
// Filename: main.ccp
//---------------------------------------------------------------------------------------

// 본 예제는 원작자의 의도와 코드 스타일, 구현 순서를 
// 철저히 동일하게 작성되었음을 알려드립니다. 

// 참고 사이트: https://www.rastertek.com/

#include "systemclass.h"
#include <crtdbg.h>

// window 응용 프로그램의 시작점
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SystemClass* System;
	bool result;

	// 시스템 객체 생성
	System = new SystemClass;

	// 시스템 객체 초기화
	result = System->Initialize();
	if (result)
	{
		// 시스템 루프 
		System->Run();
	}

	// 시스템 파괴
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}



