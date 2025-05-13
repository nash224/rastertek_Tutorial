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
	 * @brief		IsKeyDown	Ű�� ���¸� ��ȯ�Ѵ�.
	 *
	 * @param		_key	�迭�� �Ҵ���� Ű ��
	 *
	 * @return		bool	Ű�� ���� ���� ��ȯ
	 *
	 * @warning		�迭 �ε��� ��ȿ�˻縦 ���� ����
	 *				�Ű����� _Key ���� 256 ������ �ѱ� ���, ���� �߻�
	 *			
	 */
	bool IsKeyDown(unsigned int _Key) const;

protected:

private:
	char m_keys[256];

};


#endif // !__DEFINE_INPUTCLASS_H_
