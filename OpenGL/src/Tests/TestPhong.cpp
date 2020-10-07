#include "TestPhong.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

#include "ObjAttrib.h"

namespace test {

	TestPhong::TestPhong()
		: m_W(960), m_H(540),
		m_Eye(0.0f, 0.0f, 500), m_Up(0.0f, 1.0f, 0.0f), m_Amount(5), m_Center(0.0f, 0.0f, 0.0f),
		m_HighlightInt(10.0f), m_TeapotCol(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)),
		m_LightCol(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)), m_AmbientCol(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)),
		m_SpecCol(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f)), m_LightDir(glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f))
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_Proj = glm::perspective(
			90.0f * glm::pi<float>() / 180.0f, (float)m_W / (float)m_H, 0.1f, 1500.0f
		);
		m_View = glm::lookAt(m_Eye, m_Center, m_Up);

		m_Teapot = std::make_unique<SolidObject>(
			"teapot", Transform(), Mesh("res/models/cube1.obj"), "res/shaders/Phong.shader"
			);
		m_Teapot->m_Transf.scale = glm::vec3(50, 50, 50);
	}

	TestPhong::~TestPhong()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestPhong::OnUpdate(float deltaTime)
	{
		m_Teapot->m_Shader->Bind();
		m_Teapot->m_Shader->SetUniform4fv("u_Diffuse", &m_TeapotCol[0]);

		m_Teapot->m_Shader->SetUniform1f("u_SpecInt", m_HighlightInt);

		glm::vec4 light(-1.0, -1.0, -1.0, 0.0f);
		light = m_View * light;
		m_Teapot->m_Shader->SetUniform3fv("u_LightDir", &light[0]);

		m_Teapot->m_Shader->SetUniform4fv("u_Light", &m_LightCol[0]);
		m_Teapot->m_Shader->SetUniform4fv("u_Ambient", &m_AmbientCol[0]);
		m_Teapot->m_Shader->SetUniform4fv("u_Specular", &m_SpecCol[0]);

		m_View = glm::lookAt(m_Eye, m_Center, m_Up);

		m_Teapot->Update();
	}

	void TestPhong::OnRender()
	{
		GLCall(glClearColor(0, 0, 1, 0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		m_Teapot->Render(renderer, m_Proj, m_View);
	}

	void TestPhong::OnImGuiRender()
	{
		glm::vec4 camera = m_View * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		ImGui::SliderFloat3("Model", &m_Teapot->m_Transf.position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("Scale", &m_Teapot->m_Transf.scale.x, 0.0f, 100.0f);
		ImGui::ColorEdit4("Model Color", &m_TeapotCol[0]);

		ImGui::SliderFloat("Highlight Intensity", &m_HighlightInt, 0.0f, 50.0f);
		ImGui::ColorEdit4("Light Color", &m_LightCol[0]);
		ImGui::ColorEdit4("Specular Color", &m_SpecCol[0]);
		ImGui::ColorEdit4("Ambient Color", &m_AmbientCol[0]);

		ImGui::Text("Camera Position (%f, %f, %f)", -camera.x, -camera.y, -camera.z);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// This function gets called when the window size gets changed
	void TestPhong::WindowSizeCallback(GLFWwindow* window, int width, int height) {
		m_W = width;
		m_H = height;
		glViewport(0, 0, m_W, m_H);
		// Set the projection matrix based on the current perspective, then pass
		// the calculated matrix onto the shader program. Remember that the perspective
		// field of view should be in radians!
		m_Proj = glm::perspective(
			90.0f * glm::pi<float>() / 180.0f, (float)m_W / (float)m_H, 0.1f, 1500.0f
		);
	}

	void TestPhong::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			m_Lmb = action;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			m_Rmb = action;
			break;
		}
	}

	void TestPhong::MousePosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		float mousedx = m_Mousex - (float)xpos;
		float mousedy = m_Mousey - (float)ypos;
		float speed = .03;
		float dist = glm::length(m_Eye);
		float angle;
		if (m_Lmb)
		{
			angle = atan(mousedx / dist) * speed * dist;
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
			m_Eye = rot * glm::vec4(m_Eye, 1.0f);
			m_Up = rot * glm::vec4(m_Up, 1.0f);

			angle = -atan(mousedy / dist) * speed * dist;
			m_Piv = glm::normalize(glm::cross(m_Eye, m_Up));
			m_Eye = glm::rotate(glm::mat4(1.0f), angle, m_Piv) * glm::vec4(m_Eye, 1.0f);
			m_Up = glm::cross(m_Piv, glm::normalize(m_Eye));
		}
		m_Mousex = (float)xpos;
		m_Mousey = (float)ypos;
	}

	void TestPhong::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		float amt = glm::sqrt(glm::length(m_Eye)) * 4;
		glm::vec3 en = glm::normalize(m_Eye);

		m_Eye += en * amt * (float)yoffset;
	}
}