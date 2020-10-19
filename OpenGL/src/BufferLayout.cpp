#include "BufferLayout.h"

BufferLayout::BufferLayout()
	: m_Elements(0)
{
}

BufferElement BufferLayout::operator[](unsigned int i)
{
	return m_Elements[i];
}
