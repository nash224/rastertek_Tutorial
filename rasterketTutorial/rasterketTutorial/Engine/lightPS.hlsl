
// Texture2D는 2차원 배열의 픽셀 덩어리
// 레스터라이저 단계에서 건져낸 도형의 픽셀을
// 샘플러가 텍스처와 도형의 uv에 대응되는 색상을 알려준다.

// 텍스처를 사용하며 텍스처 0번 슬롯을 참조하겠다.
// 텍스처를 슬롯에 할당하는 작업은 cpu에서 수행된다.

// 사용을 끝낸 전역 슬롯은 다음 슬롯을 사용하기 전에 빼야한다.
Texture2D shaderTexture : register(t0);
// global samplerState 0번 슬롯을 참조하겠다.
SamplerState SampleType : register(s0);

cbuffer LightBuffer
{
	float4 ambientColor;	// 환경광 색상
	float4 diffuseColor;	// 빛의 색상
	float3 lightDirection;	// 빛의 방향
	float padding;			// 성능 최적화를 위한 패딩
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
	
	// 도형의 uv좌표에 대응되는 텍셀의 색상을 샘플러가 알려준다.
	// 도형 픽셀에 그릴 색상이 결정되면 그 픽셀을 그릴 것이다.
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	// 환경광을 묘사하기 위해, 도형의 모든 평면 픽셀에 환경광 색상 최소치를 할당한다.
	color = ambientColor;
	
	// 직사 방향을 반전시킨다.
	lightDir = -saturate(lightDirection);
	
	// 빛의 방향과 도형 평면의 노말을 내적해서 빛의 세기를 구한다.
	lightIntensity = saturate(dot(lightDir, input.normal));
	
	// diffuse color에 빛의 세기를 적용시켜, 직사광의 색상을 구한다.
	// 
	// 빛의 세기가 음수일 경우, 환경광이 제대로 표현되지 않을 수 있다.
	// 따라서 빛의 세기가 0을 넘길 경우에만 직사광을 적용한다.
	if (lightIntensity > 0.0f)
	{
		color += diffuseColor * lightIntensity;
	}
	
	// 환경광으로 인해 빛의 색상이 1을 넘기지 않도록 clamp 처리
	color = saturate(color);
	
	// 텍스처에 빛을 입힌다.
	// color *= textureColor;
	
	return color;
}