

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// ���籤(directional lighting)�� �¾�� ���� �� �Ÿ����� �������� ���� �����ش� �Ͱ� ����.
// ���籤�� ������ �������� ǥ�鿡 ���� �ݻ系�� ������ ���� �ν��ϴ� ���� ���� �޶�����.
// �׷��� ���� �ݻ�Ǵ� ������ �߿��ϴ�.

// �ϻ󿡼� �����ڰ� ���� �ν��ϴ� ���� ǥ�鿡 �ݻ�Ǵ� ���� ��������,
// �׷��ȿ����� ���� ǥ���� �ȼ��� ���� �����̴�.

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL; // ���� �ݻ簢�� �˾Ƴ��� ���� ������ �븻(ǥ��) ����
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

// 
PixelInputType LightVertexShader(VertexInputType input)
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
	
	// ���� �ݻ籤�� ���Ϸ��� ���� �� �����ϴ� ������ �븻���͸� �˾ƾ��Ѵ�.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	
	// ���� ó���� �����ϱ� ���� ����ȭ ���͸� �Ѱ��ش�.
	output.normal = normalize(output.normal);
	
	return output;
}