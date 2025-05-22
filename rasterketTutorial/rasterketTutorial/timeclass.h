//***************************************************************************************
// timeclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_TIMECLASS_H_
#define __DEFINE_TIMECLASS_H_


//---------------------------------------------------------------------------------------
// INCLUDES 
//---------------------------------------------------------------------------------------
#include <windows.h>

//---------------------------------------------------------------------------------------
// Class name: spriteclass
//---------------------------------------------------------------------------------------
/*
목적:
	 프레임 간에 걸린 시간을 측정하는 클래스
	 
기능:
	- 프레임 시간 측정
*/
class TimeClass
{
public:
	// constructor & destructor
	TimeClass();
	~TimeClass();

	// delete function
	TimeClass(const TimeClass& _Other) = delete;
	TimeClass(TimeClass&& _Other) noexcept = delete;
	TimeClass& operator=(const TimeClass& _Other) = delete;
	TimeClass& operator=(TimeClass&& _Other) noexcept = delete;


	/**
	 * @brief		Initialize		리셋 기능과 같음
	 *
	 * @return		bool			시스템 질의 성공 유무
	 */
	bool Initialize();


	/**
	 * @brief		Frame			이전 프레임과 현재 프레임의 시간 차이 측정
	 *
	 * @return		void
	 *
	 * @warning		null
	 */
	void Frame();
	float GetTime();

protected:

private:
	float m_frequency;
	INT64 m_startTime;
	float m_frameTime;

};


#endif // !__DEFINE_TIMECLASS_H_
