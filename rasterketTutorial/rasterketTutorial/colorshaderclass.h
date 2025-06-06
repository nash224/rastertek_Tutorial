//***************************************************************************************
// colorshaderclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_COLORSHADERCLASS_H_
#define __DEFINE_COLORSHADERCLASS_H_


//---------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

//---------------------------------------------------------------------------------------
// Class name: ColorShaderClass
//---------------------------------------------------------------------------------------
/*
목적: 
	DirectX에서 색상 기반 쉐이더 렌더링을 처리하기 위한 클래스
	이 클래스는 정점 색상을 이용한 기하 도형의 렌더링을 위한 쉐이더 초기화
	설정, 실행, 해제를 포함한 전체 과정을 캡슐화합니다.

기능:
	- HLSL 쉐이더 파일(VS, PS)을 로드 및 컴파일
	- 입력 레이아웃, 상수 버퍼, 쉐이더 인터페이스를 생성 및 관리
	- 렌더링 전에 변환 행렬 등의 쉐이더 매개변수 설정
	- 설정된 쉐이더를 이용해 도형 렌더링 수행
*/
class ColorShaderClass
{
public:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	ColorShaderClass();
	~ColorShaderClass();


	/**
	 * @brief		Initialize		
	 *							HLSL 쉐이더 파일 로드 및 컴파일 
	 *							입력 레이아웃 초기화 
	 *							상수 버퍼 생성 
	 *
	 * @param		device		D3D11 디바이스 인터페이스
	 * @param		hwnd		메인 화면에 대한 핸들
	 *
	 * @return		bool		객체 초기화 성공 유무
	 */
	bool Initialize(ID3D11Device* device, HWND hwnd);


	/**
	 * @brief		Shutdown	쉐이더 및 d3d 객체 자원 반환
	 *
	 * @return		void
	 */
	void Shutdown();

	/**
	 * @brief		Render				매 프레임마다 세팅했던 도형을 그립니다.
	 *
	 * @param		deviceContext		D3D 렌더링 파이프라인 관리 핸들
	 * @param		indexCount			도형의 인덱스 수
	 * @param		worldMarix			월드 행렬
	 * @param		viewMarix			뷰 행렬
	 * @param		projectionMarix		투영 행렬
	 *
	 * @return		bool				상수버퍼 세팅 유무
	 */
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount,
		DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);

private:

	/**
	 * @brief		InitializeShader	경로에 존재하는 쉐이더 로드 및 생성
	 *
	 * @param		device		D3D 디바이스에 대한 핸들
	 * @param		hwnd		메인화면에 대한 핸들
	 * @param		vsFilename	버텍스 쉐이더 경로
	 * @param		psFilename	픽셀 쉐이더 경로
* 
	 *
	 * @return		bool		쉐이더 생성 시
	 *
	 * @warning		경로가 존재하지 않거나 쉐이더 컴파일이 실패하면 에러 메세지 출력
	 */
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);

	/**
	 * @brief		ShutdownShader		생성한 쉐이더 자원 반환
	 *
	 * @return		void
	 *
	 * @warning		null
	 */
	void ShutdownShader();

	/**
	 * @brief		OutputShaderErrorMessage		쉐이더 코드가 적절히 컴파일되지 않을 경우, 
	 *												에러메세지 파일 쓰기 수행
	 *
	 * @param		errorMessage		에러 메세지에 대한 버퍼
	 * @param		hwnd				에러 박스를 띄울 핸들
	 * @param		shaderFilename		컴파일 실패한 쉐이더 파일 경로
	 *
	 * @return		void
	 */
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);


	/**
	 * @brief		SetShaderParamters		상수 버퍼 세팅
	 *
	 * @param		deviceContext		D3D 렌더링 파이프라인 관리 핸들
	 * @param		worldMarix			월드 행렬
	 * @param		viewMarix			뷰 행렬
	 * @param		projectionMarix		투영 행렬
	 *
	 * @return		bool		초기화 유무
	 *
	 * @warning		상수 버퍼에 
	 */
	bool SetShaderParamters(ID3D11DeviceContext* deviceContext,
		DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);


	/**
	 * @brief		RenderShader		세팅된 쉐이더를 이용해 도형 렌더링 수행
	 *
	 * @param		deviceContext		렌더링 파이프라인을 수정할 권한
	 * @param		indexCount			그릴 도형의 인덱스 수
	 *
	 * @return		void
	 */
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;


};



#endif // !__DEFINE_COLORSHADERCLASS_H_
