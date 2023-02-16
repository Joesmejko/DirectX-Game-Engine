#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class DeviceContext;
class SwapChain
{
public:
	SwapChain(RenderSystem* system);
	//Initialize SwapChain for window
	bool init(HWND hwnd, UINT width, UINT height);
	bool present(bool vsync);
	//Release swap chain
	bool release();
private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsv;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};