//***************************************************************************************
// FpsClass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_FPSCLASS_H_
#define __DEFINE_FPSCLASS_H_


//---------------------------------------------------------------------------------------
// LINKING 
//---------------------------------------------------------------------------------------
#pragma comment(lib, "winmm.lib")

//---------------------------------------------------------------------------------------
// INCLUDES 
//---------------------------------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h>


/*
����: 
	�ʴ� �������� �����ϴ� Ŭ����

���:
	- �ʴ� �������� ����
*/
class FpsClass
{
public:
	// constructor & destructor
	FpsClass();
	~FpsClass();

	// delete function
	FpsClass(const FpsClass& _Other) = delete;
	FpsClass(FpsClass&& _Other) noexcept = delete;
	FpsClass& operator=(const FpsClass& _Other) = delete;
	FpsClass& operator=(FpsClass&& _Other) noexcept = delete;

	int GetFps() const { return m_fps; }

	void Initialize();
	void Frame();

protected:

private:
	int m_fps;
	int m_count;
	unsigned long m_startTime;

};


#endif // !__DEFINE_FPSCLASS_H_
