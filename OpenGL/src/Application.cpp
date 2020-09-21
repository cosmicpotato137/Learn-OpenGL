#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "GLLog.h"

// http://docs.gl/

int main(void)
{
    // window struct
    GLFWwindow* window;

    { // added scope to avoid close window error
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

    float positions[]{
            -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f
    };

    unsigned int indices[]{
        0, 1, 2,
        0, 2, 3
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // vertex array
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));

    // vertex buffer
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    // buffer indices to be referenced
    IndexBuffer ib(indices, 6);

    // shader to be used
    Shader shader("res/shaders/basic.shader");
    shader.Bind();

    // define texture
    Texture texture("res/textures/unnamed.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;


    float r = 0.0;
    float inc = 0.01f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        // resets color of window
        renderer.Clear();

        // update color
        float val = sin(r) / 2 + 0.5f;
        if (r >= 2 * 3.14f)
            r = 0.0f;
        else
            r += inc;

        shader.Bind();
        shader.SetUniform4f("u_Color", val, val, 1.0f, 1.0f);

        // finally, the draw call
        renderer.Draw(va, ib, shader);

        //legacy gl triangle
        //glBegin(GL_TRIANGLES);
        //glVertex2d(.3f, 0.0f);
        //glVertex2d(-.3f, 0.0f);
        //glVertex2d(0.0f, -.3f);
        //glEnd(); 

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    } // added scope to avoid close window error
    glfwTerminate();
    return 0;
}