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
����:
	 ������ ���� �ɸ� �ð��� �����ϴ� Ŭ����
	 
���:
	- ������ �ð� ����
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
	 * @brief		Initialize		���� ��ɰ� ����
	 *
	 * @return		bool			�ý��� ���� ���� ����
	 */
	bool Initialize();


	/**
	 * @brief		Frame			���� �����Ӱ� ���� �������� �ð� ���� ����
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
