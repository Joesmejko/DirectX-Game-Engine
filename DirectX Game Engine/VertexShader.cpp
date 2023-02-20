#include "VertexShader.h"
#include "RenderSystem.h"
#include <exception>

VertexShader::VertexShader(RenderSystem* system): m_system(system)
{
}

void VertexShader::release()
{
    m_vs->Release();
    delete this;
}

bool VertexShader::init(const void* shader_byte_code, size_t byte_code_size)
{
    if (!SUCCEEDED(m_system->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vs)))
        return false;

    return true;
}

