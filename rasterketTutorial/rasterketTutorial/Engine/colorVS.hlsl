

// 쉐이더 프로그래밍에서는 c에서 사용하는 타입 int, float 등을 사용 가능
// 그 외에도 변수 float4나 matrix 타입은 hlsl에서 자체적으로 지원한다.

// cbuffer(constant buffer)은  쉐이더 프로그래밍에서 사용되는 상수 변수
// 런타임 시에 매 프레임마다 cpu에서 gpu로 변수 데이터를 올려줘야 함
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// POSITION, COLOR 등은 gpu가 cpu에서 건내준 변수를 인식하기 위한 시멘틱 개념
// POSITION과 SV_POSITION의 차이점은 POSITION은 버텍스 쉐이더에서 사용되고,
// SV_POSITION은 픽셀쉐이더에서 사용

// COLOR변수를 하나 더 사용하고 싶을 경우, 시멘틱 뒤에 넘버링을 붙여야 함
// cpu에서는 몇번인지 지정, gpu에서는 변수명 + 넘버링 
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


// 버텍스 쉐이더는 cpu 에서 전달된 각 점이 2D 공간으로 투영되는 단계이다. 
// 쉐이더는 일반적으로 병렬 연산으로 gpu에서 처리된다.

// 쉐이더 코드가 컴파일되기 위해서는 cpu에서 설정한 세팅이 gpu의 입출력, 여러 시멘틱 등이 일치해야 한다.

// 버텍스 쉐이더에서 수행할 연산은 각 점을 투영 좌표계로 변환하는 것이다.
// cpu에서 보내온 각 점의 위치와 World, View, Projection 행렬을 곱하는 것이다.

PixelInputType ColorVertexShader(VertexInputType input)
{
	PixelInputType output;
	
    // 점의 w 위치 정보를 반드시 1로 설정할 것.
    // 만약 0으로 설정 시, 점의 위치가 적용되지 않음
	input.position.w = 1.0f;
	
	// 쉐이더에 내장되어있는 함수 각 점들은 성분에 맞게 내적곱을 수행한다.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.color = input.color;
	
	return output;
}