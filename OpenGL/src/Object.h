#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ObjAttrib.h"

class Object
{
public:
	Object(const std::string& name);
	~Object();

	void OnUpdate();
	void OnRender();
	void OnImGuiRender();
	
	template <typename T>
	bool DelAttrib();

	template <typename T>
	std::shared_ptr<T> GetAttrib();

	void SetAttrib(std::shared_ptr<ObjAttrib> attrib);

public:
	std::string m_Name;
private:
	std::unordered_map<std::string, std::shared_ptr<ObjAttrib>> m_Attributes;
};

//class DebugObject : public Object
//{
//public:
//	DebugObject(std::string& name, std::string& filepath, std::string& shaderpath);
//
//	void OnRender(Renderer renderer, glm::mat4 proj, glm::mat4 view);
//	void OnUpdate();
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