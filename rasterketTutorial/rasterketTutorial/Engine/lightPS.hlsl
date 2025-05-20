
// 버텍스 쉐이더와 동일하게 라이팅에 대한 정보를 매크로로 정의
// 쉐이더 내부에서 사용하는 헤더 파일을 만들고 
// 헤더 파일을 포함하는 것이 더 좋은 방법으로 보임
#define NUM_LIGHTS 4

// Texture2D는 2차원 배열의 픽셀 덩어리
// 레스터라이저 단계에서 건져낸 도형의 픽셀을
// 샘플러가 텍스처와 도형의 uv에 대응되는 색상을 알려준다.

// 텍스처를 사용하며 텍스처 0번 슬롯을 참조하겠다.
// 텍스처를 슬롯에 할당하는 작업은 cpu에서 수행된다.

// 사용을 끝낸 전역 슬롯은 다음 슬롯을 사용하기 전에 빼야한다.
Texture2D shaderTexture : register(t0);
// global samplerState 0번 슬롯을 참조하겠다.
SamplerState SampleType : register(s0);

cbuffer LightColorBuffer
{
	float4 diffuseColor[NUM_LIGHTS]; // 확산광 색상
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 lightPos[NUM_LIGHTS] : TEXCOORD1;
};

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float lightIntensity[NUM_LIGHTS];
	float4 colorArray[NUM_LIGHTS];
	float4 colorSum;
	float4 color;
	int i;
	
	// 도형의 uv좌표에 대응되는 텍셀의 색상을 샘플러가 알려준다.
	// 도형 픽셀에 그릴 색상이 결정되면 그 픽셀을 그릴 것이다.
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	// 각 광원이 합쳐진 빛의 색상을 얻는 공식은 난반사광의 공식과 매우 비슷하다.
	// 먼저 각 광원의 세기와 확산광 색상으로부터 빛의 색을 알아내고 
	// 전부 합해서 텍셀과 곱하면 된다.
	for (i = 0; i < NUM_LIGHTS; i++)
	{
		lightIntensity[i] = saturate(dot(input.lightPos[i], input.normal));
		colorArray[i] = lightIntensity[i] * diffuseColor[i];
	}
	
	colorSum = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	for (i = 0; i < NUM_LIGHTS; i++)
	{
		colorSum.r += colorArray[i].r;
		colorSum.g += colorArray[i].g;
		colorSum.b += colorArray[i].b;
	}
	
	// 텍셀에 다중광원의 최종색상을 입힌다.
	color = textureColor * saturate(colorSum);
	
	return color;
}