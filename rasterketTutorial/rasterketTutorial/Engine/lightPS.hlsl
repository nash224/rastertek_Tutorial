
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
	float specularPower;    // ���ݻ籤 ����, Ŭ���� �����Ǵ� ������ �о���
	float4 specularColor;   // ���ݻ籤 ����
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
};

// ���ݻ籤�� ������ ������ ����.
// SpecularLighting = SpecularLightColor * (ViewingDirection dot ReflectionVector) power SpecularReflectionPower
// 
// ��ü�κ��� ���� �ݻ�Ǵ� ���͸� ���ϴ� ������ ������ ����.
// ReflectionVector = 2 * LightIntensity * VertexNormal - LightDirection
// 
// ī�޶� �ٶ󺸴� ������ ī�޶� ��ġ���� ������ ��ġ�� ������ ���� �ȴ�.
// ViewingDirection = CameraPosition - VertexPosition

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
	float3 reflection;
	float4 specular;
	
	// ������ uv��ǥ�� �����Ǵ� �ؼ��� ������ ���÷��� �˷��ش�.
	// ���� �ȼ��� �׸� ������ �����Ǹ� �� �ȼ��� �׸� ���̴�.
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	// ȯ�汤�� ����� ��� �ؼ��� �ּ�ġ
	color = ambientColor;
	
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	// ���� ���� ����
	lightDir = -saturate(lightDirection);
	
	// ���� ���� = ����(���� ����, ���� ����)
	lightIntensity = saturate(dot(lightDir, input.normal));
	
	// ���ݻ籤 = ���ݻ籤 ���� * ���� ����
	// 
	// ���� ���Ⱑ ������ ���, ȯ�汤�� ����� ǥ������ ���� �� �ִ�.
	// ���� ���� ���Ⱑ 0�� �ѱ� ��쿡�� ���籤�� �����Ѵ�.
	if (lightIntensity > 0.0f)
	{
		color += diffuseColor * lightIntensity;
		
		// ȯ�汤���� ���� ���� ������ 1�� �ѱ��� �ʵ��� clamp ó����
		color = saturate(color);

		// ���� �ݻ�Ǵ� ���� ����
		reflection = normalize((2.0f * lightIntensity * input.normal) - lightDir);
		
		// �����ڰ� �ٶ󺸴� ����� ���� �ݻ�Ǵ� ������ ��ġ�� ����, ���ݻ籤�� �����ϴ� ������ �о�����.
		specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	}
	
	// �ؽ�ó�� ���� ������.
	color *= textureColor;
	
	// �ѷ��� ������ ���� �ؽ�ó ���Ŀ� ���ݻ籤�� �� ���ش�.
	color = saturate(color + specular);
	
	return color;
}