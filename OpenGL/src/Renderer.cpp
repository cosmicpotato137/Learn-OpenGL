#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    while (glGetError())
    {
        std::cout << "Clearing Errors" << std::endl;
    }
}

bool GLLogCall(const char* function, const char* file, unsigned int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") in "
            << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
