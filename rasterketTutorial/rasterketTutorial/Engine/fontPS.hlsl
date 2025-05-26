
// Texture2D�� 2���� �迭�� �ȼ� ���
// �����Ͷ����� �ܰ迡�� ������ ������ �ȼ���
// ���÷��� �ؽ�ó�� ������ uv�� �����Ǵ� ������ �˷��ش�.

// �ؽ�ó�� ����ϸ� �ؽ�ó 0�� ������ �����ϰڴ�.
// �ؽ�ó�� ���Կ� �Ҵ��ϴ� �۾��� cpu���� ����ȴ�.

// ����� ���� ���� ������ ���� ������ ����ϱ� ���� �����Ѵ�.
Texture2D shaderTexture : register(t0);
// global samplerState 0�� ������ �����ϰڴ�.
SamplerState SampleType : register(s0);

cbuffer PixelBuffer
{
	float4 pixelColor;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 FontPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color;
	
	// ������ uv��ǥ�� �����Ǵ� �ؼ��� ������ ���÷��� �˷��ش�.
	// ���� �ȼ��� �׸� ������ �����Ǹ� �� �ȼ��� �׸� ���̴�.
	color = shaderTexture.Sample(SampleType, input.tex);
	
	// �ؼ� ������ �������̶�� ����
	// ������ ������� �ʴ´�.
	if (color.r == 0.0f)
	{
		color.a = 0.0f;
	}
	else
	{
		color.a = 1.0f;
		color *= pixelColor;
	}
	
	return color;
}