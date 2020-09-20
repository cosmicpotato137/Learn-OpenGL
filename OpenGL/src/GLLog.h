#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>

#define LOG(x) std::cout << c << std::endl
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// clears all active gl errors
void GLClearError();

// calls glGetError() to find new errors
bool GLLogCall(const char* function, const char* file, unsigned int line);

// prints all active uniforms
void GLPrintUniformInfo(unsigned int program);
