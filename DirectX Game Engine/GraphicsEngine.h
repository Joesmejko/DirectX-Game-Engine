#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	//Initialize the GraphicsEngine and DirectX 11 Device
	bool init();
	//Release all the resources loaded
	bool release();
	RenderSystem* getRenderSystem();
public:
	static GraphicsEngine* get();
private:
	RenderSystem* m_render_system = nullptr;
};