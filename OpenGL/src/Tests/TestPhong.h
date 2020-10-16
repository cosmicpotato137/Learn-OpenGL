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

	class TestPhong : public Test
	{
	public:
		TestPhong();
		~TestPhong();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void WindowSizeCallback(GLFWwindow* window, int width, int height) override;
		void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) override;
		void MousePosCallback(GLFWwindow* window, double xpos, double ypos) override;
		void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) override;

	private:
		int m_H, m_W;

		float m_Mousex, m_Mousey;
		bool m_Lmb, m_Rmb;

		glm::vec3 m_Eye, m_Up, m_Piv, m_Center;
		glm::mat4 m_Proj, m_View;

		std::shared_ptr<VertexArray> m_VAO;
		std::unique_ptr<Object> m_Teapot;

		std::vector<std::shared_ptr<Object>>* m_Lights;
		std::shared_ptr<Material> m_Mat1;
	};
}
