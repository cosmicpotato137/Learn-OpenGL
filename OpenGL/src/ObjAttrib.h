#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "Shader.h"

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
	Mesh(std::string filepath);
	~Mesh();

	unsigned int Size();

private:
	void Parse();

public:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::string m_Filepath;
};

class Material : public ObjAttrib
{
public:
	Material(const std::string& shaderpath, glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 lightpos,
		glm::vec4 lightcol, float specint, glm::vec4 speccol);

	Material(const std::string& shaderpath, const std::string& matfile);
	~Material();

	void SetShaderUniforms();

private:
	void Parse(const std::string& matfile);

public:
	Shader m_Shader;
	glm::vec4 diffuseCol;
	glm::vec4 ambientCol;
	glm::vec4 lightPos;
	glm::vec4 lightCol;
	float specInt;
	glm::vec4 specCol;
};