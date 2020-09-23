#pragma once
#include "Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"


namespace test {

	class TestTransforms : public Test
	{
	public:
		TestTransforms();
		~TestTransforms();

		void OnUpdate(float deltaTime);
		void OnRender();
		void OnImGuiRender();
	private:
		glm::mat4 proj;

		glm::vec3 m_Camera;
		glm::vec3 m_ModelPos;

		VertexArray m_VertexArray;
		IndexBuffer m_IndexBuffer;
		Renderer m_Renderer;
		Shader m_Shader;

	};
}



// update color
//float val = sin(r) / 2 + 0.5f;
//if (r >= 2 * 3.14f)
//    r = 0.0f;
//else
//    r += inc;

// get the view transformation matrix
//view = glm::translate(glm::mat4(1.0f), camera);


//// set shader uniforms
//shader.Bind();
//shader.SetUniform4f("u_Color", color.x, color.y, color.z, 1.0f);

//// cherno 1
//model = glm::translate(glm::mat4(1.0f), model1Pos);
//mvp = proj * view * model;
//shader.SetUniformMat4f("u_MVP", mvp);
//renderer.Draw(va, ib, shader);

//// cherno 2
//model = glm::translate(glm::mat4(1.0f), model2Pos);
//mvp = proj * view * model;
//shader.SetUniformMat4f("u_MVP", mvp);
//renderer.Draw(va, ib, shader);
