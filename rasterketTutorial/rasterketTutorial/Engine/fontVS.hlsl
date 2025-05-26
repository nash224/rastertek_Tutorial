

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// ���� ������ �ؽ�ó�� ���������� �ؼ� ��ǥ�� �ȼ� ��ǥ�� �������Ѿ� �Ѵ�.
// ���� �����Ǵ� �ؽ�ó�� uv��ǥ�� �ȼ� ���̴��� �Ѱ��ֱ� ���ؼ� 
// float2�� TEXCOORD�� ����ϰ� 0���� ����ϵ��� �ø�ƽ ���� �ڿ� 0�� �ٿ��ش�.

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

// 
PixelInputType FontVertexShader(VertexInputType input)
{
	PixelInputType output;
	
    // ���� w ��ġ ������ �ݵ�� 1�� ������ ��.
    // ���� 0���� ���� ��, ���� ��ġ�� ������� ����
	input.position.w = 1.0f;
	
	// ���̴��� ����Ǿ��ִ� �Լ� �� ���� ��Ŀ� WVP ����� ���Ͽ� ������ķ� ��ȯ��Ų��.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	// �ȼ����̴����� ���ε� �ؽ�ó�� uv��ǥ�� �Ѱ��ش�.
	output.tex = input.tex;
	
	return output;
}