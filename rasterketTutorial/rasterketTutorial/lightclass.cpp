//***************************************************************************************
// lightclass.cpp by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#include "lightclass.h"


// 직사광(directional lighting)은 태양과 같이 먼 거리에서 평행으로 빛이 내려쬐는 것과 같다.
// 직사광은 관찰자 시점에서 표면에 빛이 반사내는 정도에 따라 인식하는 빛의 양이 달라진다.
// 그래시 빛이 반사되는 각도가 중요하다.

// 일상에서 관찰자가 빛을 인식하는 것은 표면에 반사되는 빛의 양이지만,
// 그래픽에서는 도형 표면의 픽셀과 빛의 조합이다.


// 정반사광(specular lighting)
// 물체가 조명을 받을 때, 물체의 표면으로부터 빛이 산란되어 물체의 질감을 표현하는 효과
// 관찰자 시점에 따라 빛이 강조되는 지점이 변한다.


LightClass::LightClass()
{
	m_diffuseColor = {};
	m_direction = {};
	m_ambientColor = {};
	m_specularColor = {};
	m_specularPower = 0;
}

LightClass::~LightClass()
{

}
