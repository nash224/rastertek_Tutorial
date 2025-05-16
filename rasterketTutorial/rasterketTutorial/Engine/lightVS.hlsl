

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// 직사광(directional lighting)은 태양과 같이 먼 거리에서 평행으로 빛이 내려쬐는 것과 같다.
// 직사광은 관찰자 시점에서 표면에 빛이 반사내는 정도에 따라 인식하는 빛의 양이 달라진다.
// 그래시 빛이 반사되는 각도가 중요하다.

// 일상에서 관찰자가 빛을 인식하는 것은 표면에 반사되는 빛의 양이지만,
// 그래픽에서는 도형 표면의 픽셀과 빛의 조합이다.

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
};

// 
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	
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
	
	return output;
}