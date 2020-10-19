#include "UniformBuffer.h"

#include "GLLog.h"

UniformBuffer::UniformBuffer(UniformBufferLayout layout, unsigned int binding)
	: BL(layout), m_Binding(binding)
{
	// create the uniform BUFFER and bind it to the correct binding point
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, layout.GetStride(), NULL, GL_DYNAMIC_DRAW));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID));
}

UniformBuffer::~UniformBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void UniformBuffer::BindUniformBlock(const std::string& name, std::shared_ptr<Shader> shader)
{
	shader->SetUniformBlockIndex(name, m_Binding);
}

void UniformBuffer::SetBufferSubData(unsigned int index, const void* data)
{
	// bind buffer
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));

	// get buffer index size
	unsigned int size = BufferElement::GetSizeOfType(BL[index].type);
	// make sure data will stay within the index size
	ASSERT(sizeof(data) <= size);
	// write to buffer
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, BL[index].offset, size, data));

	// make sure to tell OpenGL we're done with the pointer
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void UniformBuffer::GetBufferSubData(unsigned int index, void* data)
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
	unsigned int size = BufferElement::GetSizeOfType(BL[index].type);
	glGetBufferSubData(GL_UNIFORM_BUFFER, 0, 4, data);
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}
