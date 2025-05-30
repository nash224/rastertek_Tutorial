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
#include "fpsclass.h"
#include "inputclass.h"
#include "spriteclass.h"
#include "textureshaderclass.h"

//---------------------------------------------------------------------------------------
// GLOBALS
//---------------------------------------------------------------------------------------

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;


//---------------------------------------------------------------------------------------
// Class name: ApplicationClass
//---------------------------------------------------------------------------------------

// �׷��� �������� ���� ������ ���� ó���� ����Ѵ�.

class ApplicationClass
{
public:
	ApplicationClass();
	~ApplicationClass();


	/**
	 * @brief		Initialize	���ø����̼� �ʱ�ȭ �Լ�
	 *
	 * @param		screenWidth		ȭ�� ���� ũ��
	 * @param		screenHieght	ȭ�� ���� ũ��
	 * @param		hwnd			����ȭ�� �ڵ�
	 *
	 * @return		bool		���������� �ʱ�ȭ�ߴٸ� true
	 *
	 * @warning		null
	 */
	bool Initialize(int screenWidth, int screenHieght, HWND hwnd);

	/**
	 * @brief		Shutdown	���ø����̼��� �����Ѵ�.
	 *
	 * @return		void
	 */
	void Shutdown();

	/**
	 * @brief		Frame		���忡 �����ϴ� ������Ʈ�� ���¸� �����Ѵ�.
	 * 
	 * @param		Input		SystemClass�� �Է� �ý��� ��ü
	 *
	 * @return		void
	 */
	bool Frame(InputClass* Input);

private:

	/**
	 * @brief		Render		ȭ�鿡 ��ü�� �׸���
	 * 
	 * @return		bool		������ �߿� �߻��� ���� ����
	 */
	bool Render();

	bool UpdateFps();
	bool UpdateMouseStrings(int mouseX, int mouseY, bool mouseDown);

private:
	D3DClass* m_Direct3D; // �������� ���� d3d ��ü
	CameraClass* m_Camera;
	FontShaderClass* m_FontShader;
	FontClass* m_Font;
	TextClass* m_FpsString;
	TextClass* m_MouseString;
	FpsClass* m_Fps;
	SpriteClass* m_Cursor;
	TextureShaderClass* m_TextureShader;
	

	int m_previousFps;

};

#endif // !__DEFINE_APLLICATIONCLASS_H_
