#pragma once
#include "Shader.h"
#include "BufferLayout.h"

class UniformBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Binding;
	unsigned int m_NumElements;
	UniformBufferLayout BL;

public:
	UniformBuffer(UniformBufferLayout layout, unsigned int layoutcount, unsigned int binding);
	~UniformBuffer();

	void BindUniformBlock(const std::string& name, std::shared_ptr<Shader> shader);
	void SetBufferSubData(unsigned int x, unsigned int y, const void* data);
	void GetBufferSubData(unsigned int x, unsigned int y, void* data);
};

// glGenBuffers => glBufferData => glMapBuffer =>
