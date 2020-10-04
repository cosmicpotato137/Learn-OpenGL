#pragma once

#include <functional>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLLog.h"
#include "imgui/imgui.h"

namespace test {

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		virtual void WindowSizeCallback(GLFWwindow* window, int width, int height) 
		{ GLCall(glViewport(0, 0, width, height)); }

		virtual void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{ if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, 1); }

		virtual void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {}

		virtual void MousePosCallback(GLFWwindow* window, double xpos, double ypos) {}

		virtual void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnRender() override;
		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering Test" << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};
}