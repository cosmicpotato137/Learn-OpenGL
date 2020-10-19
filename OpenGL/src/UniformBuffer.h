#pragma once
#include "Shader.h"
#include "BufferLayout.h"

class UniformBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Binding;
	UniformBufferLayout BL;

public:
	UniformBuffer(UniformBufferLayout layout, unsigned int binding);
	~UniformBuffer();

	void BindUniformBlock(const std::string& name, std::shared_ptr<Shader> shader);
	void SetBufferSubData(unsigned int index, const void* data);
	void GetBufferSubData(unsigned int index, void* data);
};

// glGenBuffers => glBufferData => glMapBuffer =>
