#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class ObjAttrib
{
public:
	ObjAttrib() {};
	~ObjAttrib() {};

	virtual void Update() {};
};

class Transform : public ObjAttrib
{
public:
	Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(0));
	~Transform();

	glm::vec3 ApplyTransf(glm::vec4 vec);
	void ApplyTransfInpl(glm::vec4& vec);

	glm::vec3 operator*(glm::vec4 vec);
	//glm::mat4 operator*(glm::mat4 mat4);

	void UpdateTransform()
	{
		glm::mat4 sca = glm::scale(glm::mat4(1), scale);
		glm::mat4 pos = glm::translate(glm::mat4(1), position);
		
		transform = sca * pos;
	}
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::mat4 transform;

private:
};

class Mesh : public ObjAttrib
{
public:
	Mesh(const char* filepath);
	~Mesh();

	unsigned int Size();

private:
	void Parse();

public:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::string m_Filepath;
};

//class Material
//{
//public:
//	Material();
//	~Material();
//
//public:
//	Shader m_Shader;
//	glm::vec4 m_Diffuse;
//	glm::vec4 m_Ambient;
//	glm::vec4 m_;
//	glm::vec4 m_Ambient;
//
//};