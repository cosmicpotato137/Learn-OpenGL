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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/example_glfw_opengl3/imgui_impl_glfw.h"
#include "imgui/example_glfw_opengl3/imgui_impl_opengl3.h"


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
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
            0.0f,   0.0f,   0.0f, 0.0f,
            100.0f, 0.0f,   1.0f, 0.0f,
            100.0f, 100.0f, 1.0f, 1.0f,
            0.0f,   100.0f, 0.0f, 1.0f
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

    // projection matrix
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

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

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    const char* glsl_version = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    glm::mat4 mvp;

    glm::mat4 view;
    glm::vec3 camera(-100.0f, 0.0f, 0.0f);

    glm::mat4 model;
    glm::vec3 model1Pos(200.0f, 200.0f, 0.0f);
    glm::vec3 model2Pos(200.0f, 200.0f, 0.0f);
    glm::vec3 color(1.0f, 1.0f, 1.0f);
    float r = 0.0;
    float inc = 0.01f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        // resets color of window
        renderer.Clear();

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {   
            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat3("Camera", &camera.x, -1000.0f, 1000.0f);
            ImGui::SliderFloat3("Model 1", &model1Pos.x, 0.0f, 960.0f);
            ImGui::SliderFloat3("Model 2", &model2Pos.x, 0.0f, 960.0f);
            ImGui::ColorEdit3("Color", &color.r);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // imgui rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        

        // update color
        //float val = sin(r) / 2 + 0.5f;
        //if (r >= 2 * 3.14f)
        //    r = 0.0f;
        //else
        //    r += inc;

        // get the view transformation matrix
        view = glm::translate(glm::mat4(1.0f), camera);


        // set shader uniforms
        shader.Bind();
        shader.SetUniform4f("u_Color", color.x, color.y, color.z, 1.0f);

        // cherno 1
        model = glm::translate(glm::mat4(1.0f), model1Pos);
        mvp = proj * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);

        // cherno 2
        model = glm::translate(glm::mat4(1.0f), model2Pos);
        mvp = proj * view * model;
        shader.SetUniformMat4f("u_MVP", mvp);
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

    // cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}