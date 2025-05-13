

// ���̴� ���α׷��ֿ����� c���� ����ϴ� Ÿ�� int, float ���� ��� ����
// �� �ܿ��� ���� float4�� matrix Ÿ���� hlsl���� ��ü������ �����Ѵ�.

// cbuffer(constant buffer)��  ���̴� ���α׷��ֿ��� ���Ǵ� ��� ����
// ��Ÿ�� �ÿ� �� �����Ӹ��� cpu���� gpu�� ���� �����͸� �÷���� ��
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// POSITION, COLOR ���� gpu�� cpu���� �ǳ��� ������ �ν��ϱ� ���� �ø�ƽ ����
// POSITION�� SV_POSITION�� �������� POSITION�� ���ؽ� ���̴����� ���ǰ�,
// SV_POSITION�� �ȼ����̴����� ���

// COLOR������ �ϳ� �� ����ϰ� ���� ���, �ø�ƽ �ڿ� �ѹ����� �ٿ��� ��
// cpu������ ������� ����, gpu������ ������ + �ѹ��� 
// ex) COLOR0, COLOR2

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


// ���ؽ� ���̴��� cpu ���� ���޵� �� ���� 2D �������� �����Ǵ� �ܰ��̴�. 
// ���̴��� �Ϲ������� ���� �������� gpu���� ó���ȴ�.

// ���̴� �ڵ尡 �����ϵǱ� ���ؼ��� cpu���� ������ ������ gpu�� �����, ���� �ø�ƽ ���� ��ġ�ؾ� �Ѵ�.

// ���ؽ� ���̴����� ������ ������ �� ���� ���� ��ǥ��� ��ȯ�ϴ� ���̴�.
// cpu���� ������ �� ���� ��ġ�� World, View, Projection ����� ���ϴ� ���̴�.

PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;
	
    // ���� w ��ġ ������ �ݵ�� 1�� ������ ��.
    // ���� 0���� ���� ��, ���� ��ġ�� ������� ����
	input.position.w = 1.0f;
	
	// ���̴��� ����Ǿ��ִ� �Լ� �� ������ ���п� �°� �������� �����Ѵ�.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.color = input.color;
	
	return output;
}