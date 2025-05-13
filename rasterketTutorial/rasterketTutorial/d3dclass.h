//***************************************************************************************
// d3dclass.h by nash224 Visual Studio (C++) 2025 All Rights Reserved.
//***************************************************************************************

#pragma once

#ifndef __DEFINE_D3DCLASS_H_
#define __DEFINE_D3DCLASS_H_


//---------------------------------------------------------------------------------------
// LINKING
//---------------------------------------------------------------------------------------

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")


//---------------------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------------------

#include <d3d11.h>
#include <directxmath.h>
// �̰� ��ŭ�� �������ϰڽ��ϴ�..
// using namespace DirectX;


//---------------------------------------------------------------------------------------
// Class name: D3DClass
//---------------------------------------------------------------------------------------


class D3DClass
{
public:
	// constructor & destructor
	D3DClass();
	~D3DClass();

	// delete function
	D3DClass(const D3DClass& _Other) = delete;
	D3DClass(D3DClass&& _Other) noexcept = delete;
	D3DClass& operator=(const D3DClass& _Other) = delete;
	D3DClass& operator=(D3DClass&& _Other) noexcept = delete;

	ID3D11Device* GetDevice() const { return m_device; }
	ID3D11DeviceContext* GetDeviceContext() const { return m_deviceContext; }

	/**
	 * @brief		Initialize		���̷�Ʈ 3D �ʱ�ȭ
	 *
	 * @param		screenWidth		ȭ�� ���� ����
	 * @param		screenHeight	ȭ�� ���� ����
	 * @param		vsync			��������ȭ ����
	 * @param		hwnd			����ȭ�� �ڵ�
	 * @param		fullscreen		��üȭ�� ��� ����
	 * @param		screenDepth		ī�޶� ���� �Ÿ�
	 * @param		screenNear		ī�޶�� ���� ����ü �Ÿ�
	 *
	 * @return		bool		�ʱ�ȭ ���� ����
	 *
	 * @warning		GPU�� ���� ���, false��ȯ
	 */
	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, 
					bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();


	/**
	 * @brief		BeginScene		����ۿ� ���̹��۸� �ʱ�ȭ�Ѵ�.
	 *
	 * @param		float	red		���� �ȼ�
	 * @param		float	green	��� �ȼ�
	 * @param		float	blue	û�� �ȼ�
	 * @param		float	alpha	����
	 *
	 * @return		void
	 */
	void BeginScene(float red, float green, float blue, float alpha);


	/**
	 * @brief		EndScene		�׷��� ���۸� ȭ�鿡 ����Ѵ�.
	 *
	 * @return		void
	 *
	 * @warning		null
	 */
	void EndScene();

protected:

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_WorldMatrix;
	DirectX::XMMATRIX m_orthoMatrix;
	D3D11_VIEWPORT m_viewport;
	

};


#endif // !__DEFINE_D3DCLASS_H_
