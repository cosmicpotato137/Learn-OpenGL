#pragma once

#include "Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Object.h"

namespace test {

	class Test3D : public Test
	{
	public:
		Test3D();
		~Test3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void WindowSizeCallback(GLFWwindow* window, int width, int height) override;
		void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override;

	private:
		int m_H, m_W;

		glm::vec3 m_Eye, m_Up, m_Center;
		float m_Amount;
		glm::mat4 m_Proj, m_View;
		std::unique_ptr<Object> m_Teapot;
	};
}
 