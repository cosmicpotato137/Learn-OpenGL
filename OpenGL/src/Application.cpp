#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <math.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

// http://docs.gl/

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
    while (glGetError())
    {
        std::cout << "help" << std::endl;
    }
}

static bool GLLogCall(const char* function, const char* file, unsigned int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") in "
            << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[int(type)] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    // create an empty shader object to hold shader strings
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    // add source code for shader
    glShaderSource(id, 1, &src, nullptr);
    // compile the shader
    glCompileShader(id);

    // if the shader compiled or not
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        // get number of characters in the info log
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // allocate memory on the stack for chars
        char* message = (char*)_malloca(length * sizeof(char));
        // assigns the message to be called
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to comple " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}


static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attaches shaders to the current program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // deletes used shaders
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    // window struct
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    // width, height, name, fullscreen, ...
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        // close the library
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[]{
             0.5f, 0.5f,
             -0.5f, 0.5f,
             0.5f, -0.5f,
             -0.5f, -0.5f
        };

        unsigned int indices[]{
            0, 1, 2,
            1, 2, 3
        };

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // pushes data to the target buffer
        GLCall(glEnableVertexAttribArray(0));
        // formatting of data in current buffer
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, sizeof(float) * 2, 0));

        IndexBuffer ib(indices, 6);

        ShaderProgramSource source = ParseShader("res/shaders/basic.shader");

        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));

        // get location of u_Color
        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        // set u_Color

        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

        float r = 0.0;
        float inc = 0.01f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            // resets color of window
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // finally, the draw call
            //glDrawArrays(GL_TRIANGLES, 0, 3);
            GLCall(glUseProgram(shader));

            float val = sin(r) / 2 + 0.5f;
            GLCall(glUniform4f(location, 0.0f, val, val, 1.0f));

            va.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r >= 2 * 3.14f)
                r = 0.0f;
            else
                r += inc;

            //legacy gl triangle
            //glBegin(GL_TRIANGLES);
            //glVertex2d(.3f, 0.0f);
            //glVertex2d(-.3f, 0.0f);
            //glVertex2d(0.0f, -.3f);
            //glEnd();

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
        GLCall(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}