

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// �ȼ� ���̴��� ������ �ȼ��� �����ϴ� �ܰ�
// �ȼ� ���̴��� �Է��� PixelInputType�̰�, 
// position�� �� �ȼ��� ���� ��ġ
// color�� �ȼ��� ���� ���� ����
// ��°��� RGBA�� float4�̸�, ���ۿ� ��Ÿ�� ������ �ǹ���

float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	return input.color;
}