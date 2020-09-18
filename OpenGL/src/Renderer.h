#pragma once

#include <GL/glew.h>
#include <iostream>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};
