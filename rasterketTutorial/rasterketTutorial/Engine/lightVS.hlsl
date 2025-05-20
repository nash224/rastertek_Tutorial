

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

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
	float3 viewDireciton : TEXCOORD1; // ī�޶� ��ü�� ǥ���� �ٶ󺸴� ����
};

// 
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;
	
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
	
	// ���ݻ籤 ������ �ʿ��� ������ �ȼ� ���̴��� �Ѱ��ش�.
	// ���� ���̴����� �̸� ����ϸ� �ȼ� ���̴����� 
	// �� �ȼ��� �������� �ʾƵ� �Ǽ� ���� ����� ���� �� �ִ�.
	worldPosition = mul(input.position, worldMatrix);
	
	output.viewDireciton = cameraPosition.xyz - worldPosition.xyz;
	output.viewDireciton = normalize(output.viewDireciton);
	
	return output;
}