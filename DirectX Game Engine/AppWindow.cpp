#include "AppWindow.h"
#include <windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

void AppWindow::updateQuadPosition()
{
	constant cc;
	cc.m_time = ::GetTickCount64();
	m_delta_pos += m_delta_time / 10.0f;

	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;

	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 0.55f;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1, 1, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));
	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5, -1.5, 0), Vector3D(1.5, 1.5, 0), m_delta_pos));
	//cc.m_world *= temp;

	cc.m_world.setScale(Vector3D(1, 1, 1));

	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;

	cc.m_view.setIdentity();
	cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f,
		4.0f
	);

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	Window::onCreate();


	InputSystem::get()->addListener(this);

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex vertex_list[] =
	{
		//X Y Z
		//Front face
		{Vector3D(-0.5f, -0.5f, -0.5f),		Vector3D(1,0,0),	Vector3D(1,0,0)},//POS1
		{Vector3D(-0.5f, 0.5f, -0.5f),		Vector3D(0,1,0),	Vector3D(0.1,0.8,0.6)},//POS2
		{Vector3D(0.5f, 0.5f, -0.5f),		Vector3D(0,0,1),	Vector3D(1,0.6,0.8)},//POS3
		{Vector3D(0.5f, -0.5f, -0.5f),		Vector3D(1,1,0),	Vector3D(0,0.2,0.3)},//POS4

		//Back face
		{Vector3D(0.5f, -0.5f, 0.5f),		Vector3D(1,0,0),	Vector3D(1,0,0)},//POS1
		{Vector3D(0.5f, 0.5f, 0.5f),		Vector3D(0,1,0),	Vector3D(0.1,0.8,0.6)},//POS2
		{Vector3D(-0.5f, 0.5f, 0.5f),		Vector3D(0,0,1),	Vector3D(1,0.6,0.8)},//POS3
		{Vector3D(-0.5f, -0.5f, 0.5f),		Vector3D(1,1,0),	Vector3D(0,0.2,0.3)},//POS4
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(vertex_list);


	unsigned int index_list[] =
	{
		//Front side
		0,1,2,	//First triangle
		2,3,0, //Second triagle
		//Back side
		4,5,6,
		6,7,4,
		//Top side
		1,6,5,
		5,2,1,
		//Bottom side
		7,0,3,
		3,4,7,
		//Right side
		3,2,5,
		5,4,3,
		//Left side
		7,6,1,
		1,0,7
	};

	m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->load(index_list, size_index_list);

	void* shader_byte_code = nullptr;

	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl","vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(vertex_list, sizeof(vertex), size_list, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->Update();

	//Clear render target
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);

	//Nastavit viewport na render target funkcie draw
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	
	updateQuadPosition();


	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//Nastavenie default shaderov na draw finkciu
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	//Set the verticies to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	//Set the indices to draw
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);


	//Draw
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(),0, 0);
	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount64();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_ib->release();
	m_cb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		m_rot_x += 0.707f * m_delta_time;
	}

	else if (key == 'S')
	{
		m_rot_x -= 0.707f * m_delta_time;
	}

	else if (key == 'A')
	{
		m_rot_y += 0.707f * m_delta_time;
	}

	else if (key == 'D')
	{
		m_rot_y -= 0.707f * m_delta_time;
	}
}

void AppWindow::onKeyUp(int key)
{
}