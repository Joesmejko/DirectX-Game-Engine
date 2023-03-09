#include "GraphicsEngine.h"
#include "RenderSystem.h"

bool GraphicsEngine::init()
{
	try
	{
		m_render_system = new RenderSystem();
	}
	catch (...){}
	
	return true;
}


bool GraphicsEngine::release()
{	
	m_render_system->release();
	delete m_render_system;
	return true;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_render_system;
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}