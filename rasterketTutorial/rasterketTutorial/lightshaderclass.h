//***************************************************************************************
// LightShaderClass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_LIGHTSHADERCLASS_H_
#define __DEFINE_LIGHTSHADERCLASS_H_

//---------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

//---------------------------------------------------------------------------------------
// Class name: LightShaderClass
//---------------------------------------------------------------------------------------
/*
����:
	�ؽ�ó�� �� ��� ���̴� �������� ó���ϱ� ���� Ŭ����
	�� Ŭ������ �ؽ�ó�� ���� �̿��� ���� ������ �������� ����
	���̴� �ʱ�ȭ, ����, ������ ������ ��ü ������ ĸ��ȭ�մϴ�.

���:
	- HLSL ���̴� ����(VS, PS)�� �ε� �� ������
	- �Է� ���̾ƿ�, ��� ����, ���̴� �������̽��� ���� �� ����
	- ������ ���� ��ȯ ��� ���� ���̴� �Ű����� ����
	- ������ ���̴��� �̿��� ���� ������ ����
	- ���ݻ籤, ȯ�汤, ���ݻ籤 ������
*/
class LightShaderClass
{
public:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	struct CameraBufferType
	{
		DirectX::XMFLOAT3 cameraPosition;
		float padding;
	};

	struct LightBufferType
	{
		DirectX::XMFLOAT4 ambientColor;     // ȯ�汤 ����
		DirectX::XMFLOAT4 diffuseColor;     // ���ݻ籤 ����
		DirectX::XMFLOAT3 lightDirection;   // ���ݻ籤 ����
		float specularPower;				// ���ݻ籤 ����
		DirectX::XMFLOAT4 specularColor;    // ���ݻ籤 ����
	};

	LightShaderClass();
	~LightShaderClass();

	/**
	 * @brief		Initialize
	 *							HLSL �ؽ�ó ���̴� ���� �ε� �� ������
	 *							�Է� ���̾ƿ� �ʱ�ȭ
	 *							��� ���� ����
	 *
	 * @param		device		D3D11 ����̽� �������̽�
	 * @param		hwnd		���� ȭ�鿡 ���� �ڵ�
	 *
	 * @return		bool		��ü �ʱ�ȭ ���� ����
	 */
	bool Initialize(ID3D11Device* device, HWND hwnd);

	/**
	 * @brief		Shutdown	���̴� �� d3d ��ü �ڿ� ��ȯ
	 *
	 * @return		void
	 */
	void Shutdown();

	/**
	 * @brief		Render				�� �����Ӹ��� �����ߴ� ������ �׸��ϴ�.
	 *
	 * @param		deviceContext		D3D ������ ���������� ���� �ڵ�
	 * @param		indexCount			������ �ε��� ��
	 * @param		worldMarix			���� ���
	 * @param		viewMarix			�� ���
	 * @param		projectionMarix		���� ���
	 * @param		lightDirection		���籤 ����
	 * @param		ambientColor		ȯ�汤 ����
	 * @param		diffuseColor		���ݻ籤 ����
	 * @param		cameraPosition		ī�޶� ��ġ
	 * @param		specularColor		���ݻ籤 ����
	 * @param		specularPower		���ݻ籤 ����
	 *
	 * @return		bool				������� ���� ����
	 */
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount,
		DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor,
		DirectX::XMFLOAT3 cameraPosition, DirectX::XMFLOAT4 specularColor, float specularPower);

private:

	/**
	 * @brief		InitializeShader	��ο� �����ϴ� ���̴� �ε� �� ����
	 *
	 * @param		device		D3D ����̽��� ���� �ڵ�
	 * @param		hwnd		����ȭ�鿡 ���� �ڵ�
	 * @param		vsFilename	���ؽ� ���̴� ���
	 * @param		psFilename	�ȼ� ���̴� ���
*
	 *
	 * @return		bool		���̴� ���� ��
	 *
	 * @warning		��ΰ� �������� �ʰų� ���̴� �������� �����ϸ� ���� �޼��� ���
	 */
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);

	/**
	 * @brief		ShutdownShader		������ ���̴� �ڿ� ��ȯ
	 *
	 * @return		void
	 *
	 * @warning		null
	 */
	void ShutdownShader();

	/**
	 * @brief		OutputShaderErrorMessage		���̴� �ڵ尡 ������ �����ϵ��� ���� ���,
	 *												�����޼��� ���� ���� ����
	 *
	 * @param		errorMessage		���� �޼����� ���� ����
	 * @param		hwnd				���� �ڽ��� ��� �ڵ�
	 * @param		shaderFilename		������ ������ ���̴� ���� ���
	 *
	 * @return		void
	 */
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);


	/**
	 * @brief		SetShaderParamters		��� ���� ����
	 *
	 * @param		deviceContext		D3D ������ ���������� ���� �ڵ�
	 * @param		worldMarix			���� ���
	 * @param		viewMarix			�� ���
	 * @param		projectionMarix		���� ���
	 *
	 * @return		bool		�ʱ�ȭ ����
	 *
	 * @warning		��� ���ۿ�
	 */
	bool SetShaderParamters(ID3D11DeviceContext* deviceContext,
		DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, DirectX::XMFLOAT3 lightDirection, DirectX::XMFLOAT4 ambientColor, DirectX::XMFLOAT4 diffuseColor,
		DirectX::XMFLOAT3 cameraPosition, DirectX::XMFLOAT4 specularColor, float specularPower);


	/**
	 * @brief		RenderShader		���õ� ���̴��� �̿��� ���� ������ ����
	 *
	 * @param		deviceContext		������ ������������ ������ ����
	 * @param		indexCount			�׸� ������ �ε��� ��
	 *
	 * @return		void
	 */
	void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;

};

#endif // !__DEFINE_LIGHTSHADERCLASS_H_
