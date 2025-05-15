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
#include "textureshaderclass.h"
#include "modelclass.h"
#include "cameraclass.h"


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
	 * @return		void
	 */
	bool Frame();

private:

	/**
	 * @brief		Render		ȭ�鿡 ��ü�� �׸���
	 *
	 * @return		bool		������ �߿� �߻��� ���� ����
	 */
	bool Render();

private:
	D3DClass* m_Direct3D; // �������� ���� d3d ��ü
	CameraClass* m_Camera;
	ModelClass* m_Model;
	TextureShaderClass* m_TextureShader;

};

#endif // !__DEFINE_APLLICATIONCLASS_H_
