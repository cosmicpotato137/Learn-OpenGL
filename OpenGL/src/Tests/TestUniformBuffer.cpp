#include "TestUniformBuffer.h"

#include "BufferLayout.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "ObjAttrib.h"

namespace test {

	TestUniformBuffer::TestUniformBuffer()
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_Proj = std::make_shared<glm::mat4>(
			glm::perspective(90.0f * glm::pi<float>() / 180.0f, 960.0f / 540.0f, 0.1f, 1500.0f));

		//m_Proj = std::make_shared<glm::mat4>(
		//	glm::ortho(0, 960, 0, 540));

		m_View = std::make_shared<glm::mat4>(glm::lookAt(glm::vec3(0, 0, 500), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

		// lights
		{
			//m_Lights = std::make_shared<std::vector<std::shared_ptr<Object>>>();
			m_Lights = new std::vector<std::shared_ptr<Object>>();
			std::shared_ptr<Transform> transf1 = std::make_shared<Transform>(m_View, glm::vec3(1, 1, 1));
			std::shared_ptr<Light> l1 = std::make_shared<Light>(glm::vec4(-1, -1, -1, 0), glm::vec4(1, 1, 1, 1), 1.0f);
			std::shared_ptr<Object> light1 = std::make_shared<Object>("Light 1");
			light1->SetAttrib(transf1);
			light1->SetAttrib(l1);
			m_Lights->push_back(light1);
		}

		UniformBufferLayout ubl;
		ubl.Push<glm::vec4>(1);
		ubl.Push<glm::vec4>(1);
		ubl.Push<float>(1);
		m_UBO = std::make_shared<UniformBuffer>(ubl, 0);

		m_VAO = std::make_shared<VertexArray>();

		std::shared_ptr<Transform> transf = std::make_shared<Transform>(m_View); // make transform
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::string("res/models/box.obj"), m_VAO); // make mesh
		m_Mat1 = std::make_shared<Material>("Regular", "res/shaders/debug.shader", "res/models/box.mtl", m_UBO); // make material
		std::shared_ptr<MeshRenderer> meshrenderer = std::make_shared<MeshRenderer>(m_Mat1, m_Proj); // make mesh renderer

		model = std::make_unique<Object>("teapot");
		model->SetAttrib(transf);
		model->SetAttrib(mesh);
		model->SetAttrib(meshrenderer);
		model->SetAttrib(m_Mat1);

		model->GetAttrib<Transform>()->scale = glm::vec3(100, 100, 100);

		OnUpdate(0.0f);
		OnRender();
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