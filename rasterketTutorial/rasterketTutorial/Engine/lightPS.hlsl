
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
	float4 ambientColor;	// ȯ�汤 ����
	float4 diffuseColor;	// ���� ����
	float3 lightDirection;	// ���� ����
	float padding;			// ���� ����ȭ�� ���� �е�
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
	
	// ȯ�汤�� �����ϱ� ����, ������ ��� ��� �ȼ��� ȯ�汤 ���� �ּ�ġ�� �Ҵ��Ѵ�.
	color = ambientColor;
	
	// ���� ������ ������Ų��.
	lightDir = -saturate(lightDirection);
	
	// ���� ����� ���� ����� �븻�� �����ؼ� ���� ���⸦ ���Ѵ�.
	lightIntensity = saturate(dot(lightDir, input.normal));
	
	// diffuse color�� ���� ���⸦ �������, ���籤�� ������ ���Ѵ�.
	// 
	// ���� ���Ⱑ ������ ���, ȯ�汤�� ����� ǥ������ ���� �� �ִ�.
	// ���� ���� ���Ⱑ 0�� �ѱ� ��쿡�� ���籤�� �����Ѵ�.
	if (lightIntensity > 0.0f)
	{
		color += diffuseColor * lightIntensity;
	}
	
	// ȯ�汤���� ���� ���� ������ 1�� �ѱ��� �ʵ��� clamp ó��
	color = saturate(color);
	
	// �ؽ�ó�� ���� ������.
	// color *= textureColor;
	
	return color;
}