#include "BufferLayout.h"

//-----------------------------------
//
// BufferLayout
//
//-----------------------------------

BufferLayout::BufferLayout()
	: m_ByteSize(0)
{
}

BufferElement BufferLayout::operator[](unsigned int i)
{
	return m_Elements[i];
}

//-----------------------------------
//
// VertexBufferLayout
//
//-----------------------------------

VertexBufferLayout::VertexBufferLayout()
	: BufferLayout()
{
}

//-----------------------------------
//
// UniformBufferLayout
//
//-----------------------------------

UniformBufferLayout::UniformBufferLayout()
	: BufferLayout()
{
}

