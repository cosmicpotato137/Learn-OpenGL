#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Object
{
public:
	Object(std::string name, std::string filepath, std::string shaderpath);
	~Object();

	void Render(Renderer renderer, glm::mat4 proj, glm::mat4 view);
	void Update();

protected:
	void parse();

public:
	std::string m_Name;
	std::string m_Filepath;

	std::vector<glm::vec3> m_Vertices;
	std::vector<unsigned int> m_Indices;

	glm::vec3 m_Position;
	glm::vec3 m_Scale;
	glm::mat4 m_Transform;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertBuff;
	std::unique_ptr<IndexBuffer> m_IndexBuff;
	std::unique_ptr<Shader> m_Shader;
};

//class DebugObject : public Object
//{
//public:
//	DebugObject(std::string& name, std::string& filepath, std::string& shaderpath);
//
//	void Render(Renderer renderer, glm::mat4 proj, glm::mat4 view);
//	void Update();
//
//public:
//	std::string m_Name;
//
//	std::vector<glm::vec3> m_Vertices;
//	std::vector<unsigned int> m_Indices;
//
//	glm::vec3 m_Position;
//	glm::vec3 m_Scale;
//	glm::mat4 m_Transform;
//
//	std::unique_ptr<VertexArray> m_VAO;
//	std::unique_ptr<VertexBuffer> m_VertBuff;
//	std::unique_ptr<IndexBuffer> m_IndexBuff;
//	std::unique_ptr<Shader> m_Shader;
//};