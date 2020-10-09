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

	virtual void OnUpdate() {};
	virtual void OnImGuiRender() {};
};

class Transform : public ObjAttrib
{
public:
	Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(0));
	~Transform();

	glm::vec3 ApplyTransf(glm::vec4 vec);
	void ApplyTransfInpl(glm::vec4& vec);

	void OnUpdate() override;

	void OnImGuiRender() override;

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
	void OnImGuiRender() override;

private:
	void Parse();

public:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

private:
	std::string m_Filepath;
};

class Material : public ObjAttrib
{
public:
	Material(const std::string& shaderpath, glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 lightpos,
		glm::vec4 lightcol, float specint, glm::vec4 speccol);

	Material(const std::string& shaderpath, const std::string& matfile, glm::vec4 lightdir);
	~Material();

	void OnUpdate() override;
	void OnImGuiRender() override;

private:
	void Parse(const std::string& matfile);

public:
	std::string shaderPath;
	std::unique_ptr<Shader> shader;
	glm::vec4 diffuseCol;
	glm::vec4 ambientCol;
	glm::vec4 lightDir;
	glm::vec4 lightCol;
	float specInt;
	glm::vec4 specCol;
};