#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <string>
#include <fmt/include/fmt/core.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "GLLog.h"

#include "imgui/imgui.h"
#include "imgui/example_glfw_opengl3/imgui_impl_glfw.h"
#include "imgui/example_glfw_opengl3/imgui_impl_opengl3.h"

#include "Tests/TestClearColor.h"
#include "Tests/TestTransform.h"
#include "Tests/TestTexture.h"
#include "Tests/Test3D.h"

using namespace test;


// http://docs.gl/


void ImGuiInit(GLFWwindow* window)
{
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    const char* glsl_version = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

Test* currentTest; // temporary definition
void ResizeWindow(GLFWwindow* window, int width, int height)
{
    currentTest->WindowSizeCallback(window, width, height);
}

void GetKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    currentTest->KeyCallback(window, key, scancode, action, mods);
}

void GetMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    currentTest->MouseButtonCallback(window, button, action, mods);
}

void GetMousePos(GLFWwindow* window, double xpos, double ypos)
{
    currentTest->MousePosCallback(window, xpos, ypos);
}

void GetMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    currentTest->MouseScrollCallback(window, xoffset, yoffset);
}

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

    Renderer renderer;

    ImGuiInit(window);

    TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    glfwSetFramebufferSizeCallback(window, ResizeWindow);
    glfwSetKeyCallback(window, GetKey);
    glfwSetMouseButtonCallback(window, GetMouseButton);
    glfwSetCursorPosCallback(window, GetMousePos);
    glfwSetScrollCallback(window, GetMouseScroll);
    
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTransform2D>("2D Transform");
    testMenu->RegisterTest<test::TestTexture2D>("2D Textrue");
    testMenu->RegisterTest<test::Test3D>("3D Render");

    float r = 0.0;
    float inc = 0.01f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest)
        {
            currentTest->OnUpdate(0.0f);
            currentTest->OnRender();

            ImGui::Begin("Test");
            if (currentTest != testMenu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }

        // imgui rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

        // resets color of window
        renderer.Clear();
    }

    if (currentTest != testMenu)
        delete testMenu;
    delete currentTest;
    } // added scope to avoid close window error

    // cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}