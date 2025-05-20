
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
	float specularPower;    // 정반사광 세기, 클수록 강조되는 범위가 넓어짐
	float4 specularColor;   // 정반사광 색상
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

// 정반사광의 공식은 다음과 같다.
// SpecularLighting = SpecularLightColor * (ViewingDirection dot ReflectionVector) power SpecularReflectionPower
// 
// 물체로부터 빛이 반사되는 벡터를 구하는 공식은 다음과 같다.
// ReflectionVector = 2 * LightIntensity * VertexNormal - LightDirection
// 
// 카메라가 바라보는 방향은 카메라 위치에서 정점이 위치한 지점을 빼면 된다.
// ViewingDirection = CameraPosition - VertexPosition

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
	float3 reflection;
	float4 specular;
	
	// 도형의 uv좌표에 대응되는 텍셀의 색상을 샘플러가 알려준다.
	// 도형 픽셀에 그릴 색상이 결정되면 그 픽셀을 그릴 것이다.
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	// 환경광은 평면의 모든 텍셀의 최소치
	color = ambientColor;
	
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// 직사 방향 반전
	lightDir = -saturate(lightDirection);
	
	// 빛의 세기 = 내적(빛의 방향, 법석 벡터)
	lightIntensity = saturate(dot(lightDir, input.normal));
	
	// 난반사광 = 난반사광 색상 * 빛의 세기
	// 
	// 빛의 세기가 음수일 경우, 환경광이 제대로 표현되지 않을 수 있다.
	// 따라서 빛의 세기가 0을 넘길 경우에만 직사광을 적용한다.
	if (lightIntensity > 0.0f)
	{
		color += diffuseColor * lightIntensity;
		
		// 환경광으로 인해 빛의 색상이 1을 넘기지 않도록 clamp 처리ㅁ
		color = saturate(color);

		// 빛이 반사되는 방향 정의
		reflection = normalize((2.0f * lightIntensity * input.normal) - lightDir);
		
		// 관찰자가 바라보는 방향과 빛이 반사되는 방향이 일치할 수록, 정반사광이 강조하는 범위가 넓어진다.
		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	}
	
	// 텍스처에 빛을 입힌다.
	color *= textureColor;
	
	// 뚜렷한 강조를 위해 텍스처 이후에 정반사광을 더 해준다.
	color = saturate(color + specular);
	
	return color;
}