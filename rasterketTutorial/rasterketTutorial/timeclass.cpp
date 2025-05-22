//***************************************************************************************
// timeclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "timeclass.h"

TimeClass::TimeClass()
{
	m_frequency = 0;
	m_startTime = 0;
	m_frameTime = 0;
}

TimeClass::~TimeClass()
{

}

bool TimeClass::Initialize()
{
	INT64 frequency;

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if (frequency == 0)
	{
		return false;
	}

	m_frequency = (float)frequency;

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

void TimeClass::Frame()
{
	INT64 currentTime;
	INT64 elapsedTicks;;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	elapsedTicks = currentTime - m_startTime;

	m_frameTime = (float)elapsedTicks / m_frequency;
	m_startTime = currentTime;
}

float TimeClass::GetTime()
{
	return m_frameTime;
}
