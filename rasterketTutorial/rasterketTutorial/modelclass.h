//***************************************************************************************
// modelclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_modelclass_H_
#define __DEFINE_modelclass_H_


//---------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------
#include <d3d11.h>
#include <directxmath.h>


//---------------------------------------------------------------------------------------
// Geometry Shape struct 
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// Class name: ModelClass
//---------------------------------------------------------------------------------------
/*
����:
	���� ������ ������ ��� �ִ� Ŭ����
	���� ������ ǥ���ϱ� ���� ���ؽ� ���� �� �ε��� ���۸� 
	���� �� ����, ������ ������ ������ ĸ��ȭ�մϴ�.

���:
	- ���ؽ� ���̴� �� �ȼ� ���̴� ���� �� ����
	- ������ ���� ���� ������ �׸��� ����� �Է� ���ձ� ����
*/
class ModelClass
{
public:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	ModelClass();
	~ModelClass();

	int GetIndexCount() const { return m_indexCount; }

	bool Initialize(ID3D11Device* device);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

private:

	/**
	 * @brief		InitializeBuffers		���� ������ ǥ���ϱ� ���� 
											���ؽ� ���� �� �ε��� ���� ����
	 *
	 * @param		device		D3D Device �ڵ�
	 *
	 * @return		bool		������ ���� ����
	 */
	bool InitializeBuffers(ID3D11Device* device);

	/**
	 * @brief		ShutdownBuffers			���� ������ �����ϱ� ���� �ڿ��� ��ȯ�Ѵ�.
	 *
	 * @return		void
	 */
	void ShutdownBuffers();

	/**
	 * @brief		RenderBuffers			���� ������ �ε��� ���� �� ���ؽ� ���� ����,
											���� ������ �׸��� ��� ����
	 *
	 * @param		deviceContext			�𵨿� ���� ���� ������ ������ ���������ο� ������ �ڵ�
	 *
	 * @return		void
	 */
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	int m_vertexCount;
	int m_indexCount;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

};


#endif // !__DEFINE_modelclass_H_
