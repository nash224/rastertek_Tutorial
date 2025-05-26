

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// 기하 도형에 텍스처를 입히기위해 텍셀 좌표를 픽셀 좌표에 대응시켜야 한다.
// 점에 대응되는 텍스처의 uv좌표를 픽셀 쉐이더로 넘겨주기 위해서 
// float2인 TEXCOORD를 사용하고 0번을 사용하도록 시멘틱 유형 뒤에 0을 붙여준다.

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
	
    // 점의 w 위치 정보를 반드시 1로 설정할 것.
    // 만약 0으로 설정 시, 점의 위치가 적용되지 않음
	input.position.w = 1.0f;
	
	// 쉐이더에 내장되어있는 함수 각 점의 행렬에 WVP 행렬을 곱하여 투영행렬로 변환시킨다.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	// 픽셀쉐이더에서 매핑된 텍스처의 uv좌표를 넘겨준다.
	output.tex = input.tex;
	
	return output;
}