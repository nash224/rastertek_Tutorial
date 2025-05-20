

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
	float3 normal : NORMAL; // 빛의 반사각을 알아내기 위한 도형의 노말(표면) 벡터
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDireciton : TEXCOORD1; // 카메라가 물체의 표면을 바라보는 방향
};

// 
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;
	
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
	
	output.viewDireciton = cameraPosition.xyz - worldPosition.xyz;
	output.viewDireciton = normalize(output.viewDireciton);
	
	return output;
}