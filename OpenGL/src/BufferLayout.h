#pragma once

#include <vector>
#include <GL/glew.h>
#include "GLLog.h"
#include "glm/glm.hpp"

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
			case GL_FLOAT:			return sizeof(GLfloat);
			case GL_UNSIGNED_INT:	return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);
			case GL_FLOAT_VEC3:		return sizeof(glm::vec3);
			case GL_FLOAT_VEC4:		return sizeof(glm::vec4);
			case GL_FLOAT_MAT4:		return sizeof(glm::mat4);
		}
		ASSERT(false);
		return 0;
	}
};

class BufferLayout
{
protected:
	std::vector<BufferElement> m_Elements;
	unsigned int m_ByteSize;

public:
	BufferLayout();

	BufferElement operator[](unsigned int i);

	inline const std::vector<BufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetSize() const { return m_ByteSize; }
};

class VertexBufferLayout : public BufferLayout
{
public:
	VertexBufferLayout();

	template <typename T>
	void Push(unsigned int count) { static_assert(false); }

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

};

class UniformBufferLayout : public BufferLayout
{
public:
	UniformBufferLayout();

	template <typename T>
	void Push(unsigned int count) { static_assert(false); }

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, m_ByteSize, GL_FALSE });
		m_ByteSize += count * BufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<glm::vec3>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT_VEC3, count, m_ByteSize, GL_FALSE });
		m_ByteSize += count * BufferElement::GetSizeOfType(GL_FLOAT_VEC3);
	}

	template<>
	void Push<glm::vec4>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT_VEC4, count, m_ByteSize, GL_FALSE });
		m_ByteSize += count * BufferElement::GetSizeOfType(GL_FLOAT_VEC4);
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
};
