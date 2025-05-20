
// ���ؽ� ���̴��� �����ϰ� �����ÿ� ���� ������ ��ũ�η� ����
// ���̴� ���ο��� ����ϴ� ��� ������ ����� 
// ��� ������ �����ϴ� ���� �� ���� ������� ����
#define NUM_LIGHTS 4

// Texture2D�� 2���� �迭�� �ȼ� ���
// �����Ͷ����� �ܰ迡�� ������ ������ �ȼ���
// ���÷��� �ؽ�ó�� ������ uv�� �����Ǵ� ������ �˷��ش�.

// �ؽ�ó�� ����ϸ� �ؽ�ó 0�� ������ �����ϰڴ�.
// �ؽ�ó�� ���Կ� �Ҵ��ϴ� �۾��� cpu���� ����ȴ�.

// ����� ���� ���� ������ ���� ������ ����ϱ� ���� �����Ѵ�.
Texture2D shaderTexture : register(t0);
// global samplerState 0�� ������ �����ϰڴ�.
SamplerState SampleType : register(s0);

cbuffer LightColorBuffer
{
	float4 diffuseColor[NUM_LIGHTS]; // Ȯ�걤 ����
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
	
	// ������ uv��ǥ�� �����Ǵ� �ؼ��� ������ ���÷��� �˷��ش�.
	// ���� �ȼ��� �׸� ������ �����Ǹ� �� �ȼ��� �׸� ���̴�.
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	// �� ������ ������ ���� ������ ��� ������ ���ݻ籤�� ���İ� �ſ� ����ϴ�.
	// ���� �� ������ ����� Ȯ�걤 �������κ��� ���� ���� �˾Ƴ��� 
	// ���� ���ؼ� �ؼ��� ���ϸ� �ȴ�.
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
	
	// �ؼ��� ���߱����� ���������� ������.
	color = textureColor * saturate(colorSum);
	
	return color;
}