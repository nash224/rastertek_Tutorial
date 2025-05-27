//***************************************************************************************
// FpsClass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "fpsclass.h"


FpsClass::FpsClass()
{
	m_fps = 0;
	m_count = 0;

	m_startTime = 0;
}

FpsClass::~FpsClass()
{

}

void FpsClass::Initialize()
{
	m_startTime = timeGetTime();
}

void FpsClass::Frame()
{
	m_count++;

	if (timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;

		m_startTime = timeGetTime();
	}
}
