#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID)); //create one buffer and return the ID into m_RendererID
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); //say to open GL we are talk about the "m_RendererID" buffer
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Update(const void* data, unsigned int size)
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); //say to open GL we are talk about the "m_RendererID" buffer
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
