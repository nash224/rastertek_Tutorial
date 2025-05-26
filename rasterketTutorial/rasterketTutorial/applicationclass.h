//***************************************************************************************
// apllicationclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_APLLICATIONCLASS_H_
#define __DEFINE_APLLICATIONCLASS_H_


//---------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------

#include <windows.h>
#include "d3dclass.h"
#include "cameraclass.h"
#include "fontclass.h"
#include "fontshaderclass.h"
#include "textclass.h"

//---------------------------------------------------------------------------------------
// GLOBALS
//---------------------------------------------------------------------------------------

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;


//---------------------------------------------------------------------------------------
// Class name: ApplicationClass
//---------------------------------------------------------------------------------------

// 그래픽 렌더링과 게임 로직에 대한 처리를 당담한다.

class ApplicationClass
{
public:
	ApplicationClass();
	~ApplicationClass();


	/**
	 * @brief		Initialize	어플리케이션 초기화 함수
	 *
	 * @param		screenWidth		화면 가로 크기
	 * @param		screenHieght	화면 세로 크기
	 * @param		hwnd			메인화면 핸들
	 *
	 * @return		bool		정상적으로 초기화했다면 true
	 *
	 * @warning		null
	 */
	bool Initialize(int screenWidth, int screenHieght, HWND hwnd);

	/**
	 * @brief		Shutdown	어플리케이션을 정리한다.
	 *
	 * @return		void
	 */
	void Shutdown();

	/**
	 * @brief		Frame		월드에 존재하는 오브젝트의 상태를 갱신한다.
	 *
	 * @return		void
	 */
	bool Frame();

private:

	/**
	 * @brief		Render		화면에 물체를 그린다
	 * 
	 * @return		bool		렌더링 중에 발생한 오류 유무
	 */
	bool Render();

private:
	D3DClass* m_Direct3D; // 렌더링을 위한 d3d 객체
	CameraClass* m_Camera;
	FontShaderClass* m_FontShader;
	FontClass* m_Font;
	TextClass* m_TextString1;
	TextClass* m_TextString2;

};

#endif // !__DEFINE_APLLICATIONCLASS_H_
