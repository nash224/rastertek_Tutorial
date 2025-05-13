//***************************************************************************************
// inputclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_INPUTCLASS_H_
#define __DEFINE_INPUTCLASS_H_

class InputClass
{
public:
	// constructor & destructor
	InputClass();
	~InputClass();

	// delete function
	InputClass(const InputClass& _Other) = delete;
	InputClass(InputClass&& _Other) noexcept = delete;
	InputClass& operator=(const InputClass& _Other) = delete;
	InputClass& operator=(InputClass&& _Other) noexcept = delete;

	void Initialize();

	void KeyDown(unsigned int _Input);
	void KeyUp(unsigned int _Input);


	/**
	 * @brief		IsKeyDown	키의 상태를 반환한다.
	 *
	 * @param		_key	배열에 할당받은 키 값
	 *
	 * @return		bool	키의 눌림 여부 반환
	 *
	 * @warning		배열 인덱스 유효검사를 하지 않음
	 *				매개변수 _Key 값이 256 범위를 넘길 경우, 오류 발생
	 *			
	 */
	bool IsKeyDown(unsigned int _Key) const;

protected:

private:
	char m_keys[256];

};


#endif // !__DEFINE_INPUTCLASS_H_
