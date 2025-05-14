//***************************************************************************************
// ShapeClass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_ShapeClass_H_
#define __DEFINE_ShapeClass_H_


class ShapeClass
{
public:
	// constructor & destructor
	ShapeClass() {}
	~ShapeClass() {}

	// delete function
	ShapeClass(const ShapeClass& _Other) = delete;
	ShapeClass(ShapeClass&& _Other) noexcept = delete;
	ShapeClass& operator=(const ShapeClass& _Other) = delete;
	ShapeClass& operator=(ShapeClass&& _Other) noexcept = delete;

protected:

private:

};


#endif // !__DEFINE_ShapeClass_H_
