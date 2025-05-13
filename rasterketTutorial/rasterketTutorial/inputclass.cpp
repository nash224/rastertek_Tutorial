//***************************************************************************************
// inputclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "inputclass.h"

InputClass::InputClass()
	: m_keys{0}
{
}

InputClass::~InputClass()
{

}

void InputClass::Initialize()
{
	int i;

	for (i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
}

void InputClass::KeyDown(unsigned int _Input)
{
	m_keys[_Input] = true;
}

void InputClass::KeyUp(unsigned int _Input)
{
	m_keys[_Input] = false;
}

bool InputClass::IsKeyDown(unsigned int _Key) const
{
	// 키의 눌림 여부 반환
	return m_keys[_Key];
}
