
//---------------------------------------------------------------------------------------
// Filename: main.ccp
//---------------------------------------------------------------------------------------

// 본 예제는 원작자의 의도와 코드 스타일, 구현 순서를 
// 경고 또는 Leak이 발생하지 않는 선에서 
// 철저히 동일하게 작성되었음을 알려드립니다. 

// 추신: 원작자의 코드를 보면 생성에 실패 시, 
// 지역변수의 자원이 해제되지 않는 문제가 있습니다.
// 원작자의 코드 스타일을 최대한 비슷하게 유지하도록 goto 문을 사용했음을 알려드립니다.
// 예시) ModelClass::InitializeBuffers 

// 참고 사이트: https://www.rastertek.com/

// 조작법: 
//		- escape: exit application

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



