#include "TestUniformBuffer.h"

#include "BufferLayout.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "ObjAttrib.h"
#include "GLLog.h"

namespace test {

	TestUniformBuffer::TestUniformBuffer()
	{
		unsigned int m_RendererID;
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_UNIFORM_BUFFER, 152, NULL, GL_DYNAMIC_DRAW));
		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));

		int o;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &o);

		GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_RendererID, 0, 16));
		GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_RendererID, 256, 16));

	}

	TestUniformBuffer::~TestUniformBuffer()
	{
		delete m_Lights;
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestUniformBuffer::OnUpdate(float deltaTime)
	{
		model->OnUpdate();
	}

	void TestUniformBuffer::OnRender()
	{
		GLCall(glClearColor(0, 0, 1, 0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;

		model->OnRender();
	}

	void TestUniformBuffer::OnImGuiRender()
	{
		ImGui::BeginChild("Scene", ImVec2(300, 400), true);
		ImGui::Text("Scene View");
		model->OnImGuiRender();

		for (int i = 0; i < m_Lights->size(); i++)
			(*m_Lights)[i]->OnImGuiRender();

		ImGui::EndChild();
		glm::vec4 camera = *m_View * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		ImGui::Text("Camera Position (%f, %f, %f)", -camera.x, -camera.y, -camera.z);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}