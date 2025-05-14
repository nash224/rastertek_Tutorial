

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// 픽셀 쉐이더는 도형의 픽셀을 결정하는 단계
// 픽셀 쉐이더의 입력은 PixelInputType이고, 
// position은 각 픽셀에 대한 위치
// color는 픽셀에 대한 색상 정보
// 출력값은 RGBA로 float4이며, 버퍼에 나타낼 색상을 의미함

float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	float brightness = 0.5f;
	input.color.r *= brightness;
	input.color.g *= brightness;
	input.color.b *= brightness;
	return input.color;
}