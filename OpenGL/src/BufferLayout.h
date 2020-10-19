#pragma once

#include <vector>
#include <GL/glew.h>
#include "GLLog.h"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"

struct BufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned int offset;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
			//case GL_FLOAT_VEC3:		return sizeof(glm::vec3);
		}
		ASSERT(false);
		return 0;
	}
};

class BufferLayout
{
private:
	std::vector<BufferElement> m_Elements;
	unsigned int m_ByteSize;

public:
	BufferLayout();

	BufferElement operator[](unsigned int i);

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

};

class VertexBufferLayout : public BufferLayout
{
public:

	VertexBufferLayout()
		: BufferLayout()
	{
	}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, m_ByteSize, GL_FALSE });
		m_ByteSize += count * BufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<glm::vec3>(unsigned int count)
	{
		for (unsigned int i = 0; i < count; i++)
			Push<float>(3);
	}

	template<>
	void Push<glm::vec4>(unsigned int count)
	{
		for (unsigned int i = 0; i < count; i++)
			Push<float>(4);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, m_ByteSize, GL_FALSE });
		m_ByteSize += count * BufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, m_ByteSize, GL_TRUE });
		m_ByteSize += count * BufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<BufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_ByteSize; }
};

class UniformBufferLayout
{

};
