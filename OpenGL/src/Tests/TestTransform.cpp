#include "TestTransform.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

namespace test {

	TestTransform2D::TestTransform2D()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Model(200, 200, 0), m_Camera(0, 0, 0)
	{
		float positions[]{
			0.0f,   0.0f,  
			100.0f, 0.0f,  
			100.0f, 100.0f,
			0.0f,   100.0f,
		};

		unsigned int indices[]{
			0, 1, 2,
			0, 2, 3
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 1.0f, 0.5f, 1.0f, 1.0f);
	}

	TestTransform2D::~TestTransform2D()
	{
	}

	void TestTransform2D::OnUpdate(float deltaTime)
	{
	}

	void TestTransform2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		// box 1
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Model);
		m_View = glm::translate(glm::mat4(1.0f), m_Camera);

		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestTransform2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Model", &m_Model.x, -1000.0f, 1000.0f);
		ImGui::SliderFloat3("Camera", &m_Camera.x, -1000.0f, 1000.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}