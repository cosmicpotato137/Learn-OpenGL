#include "TestPhong.h"
#include "BufferLayout.h"
#include "ObjScripts/CameraMove.h"

namespace test {

	TestPhong::TestPhong()
		: m_W(960), m_H(540)
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// camera
		{
			glm::mat4 proj = glm::perspective(90.0f * glm::pi<float>() / 180.0f, (float)m_W / (float)m_H, 0.1f, 1500.0f);
			std::shared_ptr<Transform> transf = std::make_shared<Transform>(glm::vec3(0, 0, -500));
			std::shared_ptr<Camera> cam = std::make_shared<Camera>(proj, transf);
			std::shared_ptr<CameraMove> cm = std::make_shared<CameraMove>(transf);
			m_ActiveCamera = std::make_unique<Object>("Main Camera");
			m_ActiveCamera->SetAttrib(transf);
			m_ActiveCamera->SetAttrib(cam);
			m_ActiveCamera->SetAttrib(cm);
		}

		// lights
		{
			std::shared_ptr<Transform> transf1 = std::make_shared<Transform>(glm::vec3(1, 1, 1));
			std::shared_ptr<Light> l1 = std::make_shared<Light>(glm::vec4(-1, -1, -1, 0), glm::vec4(1, 1, 1, 1), 1.0f);
			m_Lights.push_back(std::make_unique<Object>("Light 1"));
			m_Lights[0]->SetAttrib(transf1);
			m_Lights[0]->SetAttrib(l1);

			std::shared_ptr<Transform> transf2 = std::make_shared<Transform>(glm::vec3(1, 1, 1));
			std::shared_ptr<Light> l2 = std::make_shared<Light>(glm::vec4(1, -1, 1, 0), glm::vec4(1, 1, 1, 1), 1.0f);
			m_Lights.push_back(std::make_unique<Object>("Light 2"));
			m_Lights[1]->SetAttrib(transf2);
			m_Lights[1]->SetAttrib(l2);
		}

		// make light buffer
		{
			UniformBufferLayout ubl;
			unsigned int buffer_size = m_Lights.size();
			ubl.Push<glm::vec4>(1);
			ubl.Push<glm::vec4>(1);
			ubl.Push<float>(1);
			m_LightBuffer = std::make_shared<UniformBuffer>(ubl, buffer_size, 0);
		}

		// teapot object
		{
			m_VAO = std::make_shared<VertexArray>();

			std::shared_ptr<Transform> transf = std::make_shared<Transform>(); // make transform
			std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(std::string("res/models/teapot.obj"), m_VAO); // make mesh
			m_Mat1 = std::make_shared<Material>("Regular", "res/shaders/Phong.shader", "res/models/teapot.mtl", m_LightBuffer); // make material
			std::shared_ptr<MeshRenderer> meshrenderer = std::make_shared<MeshRenderer>(m_Mat1, &*m_ActiveCamera); // make mesh renderer

			m_Teapot = std::make_unique<Object>("teapot");
			m_Teapot->SetAttrib(transf);
			m_Teapot->SetAttrib(mesh);
			m_Teapot->SetAttrib(meshrenderer);
			m_Teapot->SetAttrib(m_Mat1);
			m_Teapot->GetAttrib<Transform>()->scale = glm::vec3(50, 50, 50);
		}

		OnUpdate(0.0f);
		OnRender();
	}

	TestPhong::~TestPhong()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glDisable(GL_BLEND));
	}

	void TestPhong::OnUpdate(float deltaTime)
	{
		// update lights
		int i = 0;
		while (i < m_Lights.size())
		{
			m_Lights[i]->OnUpdate();
			Light light = *m_Lights[i]->GetAttrib<Light>();
			glm::vec4 dir = m_ActiveCamera->GetAttrib<Camera>()->view * light.lightDir;
			glm::vec4 col = light.active ? light.lightCol : glm::vec4(0);

			m_LightBuffer->SetBufferSubData(0, i, sizeof(dir), &dir);
			m_LightBuffer->SetBufferSubData(1, i, sizeof(col), &col);
			m_LightBuffer->SetBufferSubData(2, i, sizeof(light.lightInt), &light.lightInt);
			i++;
		}

		m_Teapot->OnUpdate();
		m_ActiveCamera->OnUpdate();
	}

	void TestPhong::OnRender()
	{
		GLCall(glClearColor(0, 0, 1, 0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_Teapot->OnRender();
	}

	void TestPhong::OnImGuiRender()
	{
		ImGui::BeginChild("Scene", ImVec2(300, 400), true);
		ImGui::Text("Scene View");
		m_Teapot->OnImGuiRender();
		m_ActiveCamera->OnImGuiRender();

		for (int i = 0; i < m_Lights.size(); i++)
			m_Lights[i]->OnImGuiRender();

		ImGui::EndChild();
		glm::vec4 camera = m_ActiveCamera->GetAttrib<Camera>()->view * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
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
		m_ActiveCamera->GetAttrib<Camera>()->projection = glm::perspective(
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
		m_ActiveCamera->GetAttrib<CameraMove>()->MouseUpdate(m_Lmb, xpos, ypos);
	}

	void TestPhong::MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_ActiveCamera->GetAttrib<CameraMove>()->ScrollUpdate(xoffset, yoffset);
	}
}