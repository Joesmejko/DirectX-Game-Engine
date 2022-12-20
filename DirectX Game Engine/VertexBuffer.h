#pragma once
#include <d3d11.h>

class VertexBuffer
{
public:
	bool load(void* list_verticies, UINT size_vertex, UINT size_list);
	bool release();
private:
	UINT m_size_vertex;
	UINT m_size_list;
private:
	ID3D11Buffer* m_buffer;
};

