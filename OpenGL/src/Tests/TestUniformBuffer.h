#pragma once

#include "Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Object.h"

namespace test {

	class TestUniformBuffer : public Test
	{
	public:
		TestUniformBuffer();
		~TestUniformBuffer();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::shared_ptr<Material> m_Mat1;
		std::unique_ptr<Object> model;
		std::shared_ptr<VertexArray> m_VAO;
		std::shared_ptr<glm::mat4> m_Proj, m_View;

		std::vector<std::shared_ptr<Object>>* m_Lights;
		std::shared_ptr<UniformBuffer> m_LightBuffer;

	};
}