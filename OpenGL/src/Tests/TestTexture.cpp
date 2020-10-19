#include "TestTexture.h"

#include "BufferLayout.h"
#include "Renderer.h"

namespace test {

	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_Model1(400, 200, 0), blend(false), dblend(false)
	{
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

		// define blend function
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/textureTest.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Model1);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		m_Texture = std::make_unique<Texture>("res/textures/dirt.png");
		m_Texture->Bind();
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
		if (!blend && blend != dblend)
		{
			GLCall(glDisable(GL_BLEND));
			dblend = blend;
		}
		else if (blend && blend != dblend)
		{
			GLCall(glEnable(GL_BLEND));
			dblend = blend;
		}
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.4f, 0.6f, 0.3f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::Checkbox("Enable Blending", &blend);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}