#include "Transforms.h"

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

namespace test {

	TestTransforms::TestTransforms()
		: m_Camera(0.0f, 0.0f, 0.0f), m_ModelPos(0.0f, 0.0f, 0.0f),
		m_IndexBuffer(indices, 6), m_Shader("res/shaders/basic.shader"), 
		proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f))
	{

		// vertex array
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		// vertex buffer
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VertexArray.AddBuffer(vb, layout);

		// shader to be used
		m_Shader.Bind();
		m_Shader.SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

		glm::mat4 view = glm::translate(glm::mat4(1.0f), m_Camera);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_ModelPos);
		glm::mat4 mvp = proj;
		//m_Shader.Bind();
		m_Shader.SetUniformMat4f("u_MVP", mvp);

		m_VertexArray.Unbind();
		vb.Unbind();
		m_IndexBuffer.Unbind();
		m_Shader.Unbind();
	}

	TestTransforms::~TestTransforms()
	{
	}

	void TestTransforms::OnUpdate(float deltaTime) 
	{

	}

	void TestTransforms::OnRender() 
	{
		m_Renderer.Draw(m_VertexArray, m_IndexBuffer, m_Shader);
		m_Renderer.Clear();
	}

	void TestTransforms::OnImGuiRender() 
	{
		// Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat3("Camera", &m_Camera.x, -1000.0f, 1000.0f);
		ImGui::SliderFloat3("Model 1", &m_ModelPos.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}