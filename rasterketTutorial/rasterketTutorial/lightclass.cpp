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

// 다중 광원(Multiple Point Lights)
// 일상 생활에서 다중광원은 여러 개의 형광등을 비추는 것과 같다.
// 광원은 위치와 색상을 가지고 있고, 중앙으로부터 멀어질 수록 빛이 소멸된다.
// 빛을 소멸시키는 방법도 여러가지로 표현할 수 있다.
// 
// 광원을 여러개 렌더링하기 위해서는 라이트 하나당 연산하기에는 무리가 있다.
// 따라서 지연 쉐이딩(deffered shading)이라는 렌더링 기법을 이용하면 어느정도 렌더링 비용을 낮출 수 있다.
// 



LightClass::LightClass()
{
	m_diffuseColor = {};
	m_direction = {};
	m_ambientColor = {};
	m_specularColor = {};
	m_specularPower = 0;
	m_position = {};
}

LightClass::~LightClass()
{

}
