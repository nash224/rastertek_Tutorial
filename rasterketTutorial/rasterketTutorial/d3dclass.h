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
// 이것 만큼은 못따라하겠습니다..
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
	 * @brief		Initialize		다이렉트 3D 초기화
	 *
	 * @param		screenWidth		화면 가로 길이
	 * @param		screenHeight	화면 세로 길이
	 * @param		vsync			수직동기화 유무
	 * @param		hwnd			메인화면 핸들
	 * @param		fullscreen		전체화면 모드 유무
	 * @param		screenDepth		카메라 깊이 거리
	 * @param		screenNear		카메라와 투영 절두체 거리
	 *
	 * @return		bool		초기화 성공 여부
	 *
	 * @warning		GPU가 없을 경우, false반환
	 */
	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, 
					bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();


	/**
	 * @brief		BeginScene		백버퍼와 깊이버퍼를 초기화한다.
	 *
	 * @param		float	red		적색 픽셀
	 * @param		float	green	녹색 픽셀
	 * @param		float	blue	청색 픽셀
	 * @param		float	alpha	투명도
	 *
	 * @return		void
	 */
	void BeginScene(float red, float green, float blue, float alpha);


	/**
	 * @brief		EndScene		그려진 버퍼를 화면에 출력한다.
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
