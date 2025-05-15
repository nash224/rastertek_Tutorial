
// Texture2D는 2차원 배열의 픽셀 덩어리
// 레스터라이저 단계에서 건져낸 도형의 픽셀을
// 샘플러가 텍스처와 도형의 uv에 대응되는 색상을 알려준다.

// 텍스처를 사용하며 텍스처 0번 슬롯을 참조하겠다.
// 텍스처를 슬롯에 할당하는 작업은 cpu에서 수행된다.

// 사용을 끝낸 전역 슬롯은 다음 슬롯을 사용하기 전에 빼야한다.
Texture2D shaderTexture : register(t0);
// global samplerState 0번 슬롯을 참조하겠다.
SamplerState SampleType : register(s0);

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;
	
	// 도형의 uv좌표에 대응되는 텍셀의 색상을 샘플러가 알려준다.
	// 도형 픽셀에 그릴 색상이 결정되면 그 픽셀을 그릴 것이다.
	color = shaderTexture.Sample(SampleType, input.tex);
	
	return color;
}