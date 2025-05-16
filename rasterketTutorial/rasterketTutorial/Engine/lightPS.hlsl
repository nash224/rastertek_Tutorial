
// Texture2D�� 2���� �迭�� �ȼ� ���
// �����Ͷ����� �ܰ迡�� ������ ������ �ȼ���
// ���÷��� �ؽ�ó�� ������ uv�� �����Ǵ� ������ �˷��ش�.

// �ؽ�ó�� ����ϸ� �ؽ�ó 0�� ������ �����ϰڴ�.
// �ؽ�ó�� ���Կ� �Ҵ��ϴ� �۾��� cpu���� ����ȴ�.

// ����� ���� ���� ������ ���� ������ ����ϱ� ���� �����Ѵ�.
Texture2D shaderTexture : register(t0);
// global samplerState 0�� ������ �����ϰڴ�.
SamplerState SampleType : register(s0);

cbuffer LightBuffer
{
	float4 diffuseColor; // ���� ����
	float3 lightDirection; // ���� ����
	float padding; // ���� ����ȭ�� ���� �е�
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
	
	// ������ uv��ǥ�� �����Ǵ� �ؼ��� ������ ���÷��� �˷��ش�.
	// ���� �ȼ��� �׸� ������ �����Ǹ� �� �ȼ��� �׸� ���̴�.
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	// ���� ������ ������Ų��.
	lightDir = -lightDirection;
	
	// ���� ����� ���� ����� �븻�� �����ؼ� ���� ���⸦ ���Ѵ�.
	lightIntensity = saturate(dot(lightDir, input.normal));
	
	// ���� Ȯ�� ����� ���� ���⸦ ���� ���籤�� ������ ���Ѵ�.
	color = saturate(diffuseColor * lightIntensity);
	
	// �ؽ�ó�� ���籤�� ������.
	color *= textureColor;
	
	return color;
}