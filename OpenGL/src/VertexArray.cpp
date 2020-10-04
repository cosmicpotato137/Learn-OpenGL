#include "VertexArray.h"
#include "GLLog.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, unsigned int lpos)
{
    Bind();
	vb.Bind();
    // list of all elements in m_Elements vector
    const auto& elements = layout.GetElements();

    unsigned int offset = 0;
    for (; lpos < elements.size(); lpos++)
    {
        const auto& element = elements[lpos];
        // pushes data to the target buffer
        GLCall(glEnableVertexAttribArray(lpos));
        // formatting of data in current 
        GLCall(glVertexAttribPointer(lpos, element.count, element.type, 
            element.normalized, layout.GetStride(), (const void*)offset));

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}