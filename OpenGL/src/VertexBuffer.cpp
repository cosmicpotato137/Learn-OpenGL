#include "VertexBuffer.h"
#include "GLLog.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int usage)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::SetBufferSubData(unsigned int offset, unsigned int size, const void* data)
{
	// bind buffer
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));

	// write to buffer
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));

	// make sure to tell OpenGL we're done with the pointer
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}