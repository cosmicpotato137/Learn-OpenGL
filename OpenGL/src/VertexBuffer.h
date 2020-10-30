#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void* data, unsigned int size, unsigned int usage);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetBufferSubData(unsigned int offset, unsigned int size, const void* data);
};

