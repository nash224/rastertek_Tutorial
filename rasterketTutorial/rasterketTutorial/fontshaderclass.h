//***************************************************************************************
// fontshaderclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_FONTSHADERCLASS_H_
#define __DEFINE_FONTSHADERCLASS_H_

//---------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

//---------------------------------------------------------------------------------------
// Class name: FontShaderClass
//---------------------------------------------------------------------------------------
/*
����:
	�ؽ�Ʈ�� ����ϱ� ���� Ŭ����
	�ؽ�Ʈ�� �׸��� ���� ���̴�, ��� ����, �ؽ�ó ���ҽ��� 
	����, ����, ������ ����ϰ� ���� ������ �����͸� �����մϴ�.

���:
	- HLSL ���̴� ����(VS, PS)�� �ε� �� ������
	- �Է� ���̾ƿ�, ��� ����, ���̴� �������̽��� ���� �� ����
	- ������ ���� ��ȯ ��� ���� ���̴� �Ű����� ����
	- �ؽ�Ʈ�� ����Ÿ�ٿ� Draw
*/
class FontShaderClass
{
public:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	// ���� ����
	struct PixelBufferType
	{
		DirectX::XMFLOAT4 pixelColor;
	};

	FontShaderClass();
	~FontShaderClass();


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
	 * @param		pixelColor			��Ʈ ����
	 *
	 * @return		bool				������� ���� ����
	 */
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount,
		DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix,
		ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4 pixelColor);

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
	 * @param		pixelColor			��Ʈ ����
	 *
	 * @return		bool		�ʱ�ȭ ����
	 *
	 * @warning		��� ���ۿ�
	 */
	bool SetShaderParamters(ID3D11DeviceContext* deviceContext,
		DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix,
		ID3D11ShaderResourceView* texture, DirectX::XMFLOAT4 pixelColor);


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
	ID3D11Buffer* m_pixelBuffer;
	ID3D11SamplerState* m_sampleState;

};

#endif // !__DEFINE_FONTSHADERCLASS_H_
