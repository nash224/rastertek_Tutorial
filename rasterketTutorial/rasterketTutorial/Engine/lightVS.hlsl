
// 다중 광원을 구현하기 위해서는 광원의 정보를 담고 있는 배열이 필요하다.
// 정적 배열을 할당하기 위해 광원의 수를 정의한다.
#define NUM_LIGHTS 4

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// 광원에 대한 정보
cbuffer LightPositionBuffer
{
	float4 lightPosition[NUM_LIGHTS];
};

// 이번 예제에서는 정반사광을 다루지 않기 때문에 주석처리한다.
//cbuffer CameraBuffer
//{
//	float3 cameraPosition;
//	float padding;
//};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL; 
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 lightPos[NUM_LIGHTS] : TEXCOORD1;
};

// 
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;
	int i;
	
    // 점의 w 위치 정보를 반드시 1로 설정할 것.
    // 만약 0으로 설정 시, 점의 위치가 적용되지 않음
	input.position.w = 1.0f;
	
	// 쉐이더에 내장되어있는 함수 각 점의 행렬에 WVP 행렬을 곱하여 투영행렬로 변환시킨다.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	// 픽셀쉐이더에서 매핑된 텍스처의 uv좌표를 넘겨준다.
	output.tex = input.tex;
	
	// 빛의 반사광을 구하려면 월드 상에 존재하는 도형의 노말벡터를 알아야한다.
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	
	// 보간 처리를 방지하기 위해 정규화 벡터를 넘겨준다.
	output.normal = normalize(output.normal);
	
	// 난반사광 구현에 필요한 변수를 픽셀 쉐이더로 넘겨준다.
	// 점정 쉐이더에서 미리 계산하면 픽셀 쉐이더에서 
	// 각 픽셀당 연산하지 않아도 되서 연산 비용을 줄일 수 있다.
	worldPosition = mul(input.position, worldMatrix);
	
	for (i = 0; i < NUM_LIGHTS; i++)
	{
		// 각 정점이 빛을 바라보는 방향
		output.lightPos[i] = lightPosition[i].xyz - worldPosition.xyz;
		output.lightPos[i] = normalize(output.lightPos[i]);
	}
	
		return output;
}