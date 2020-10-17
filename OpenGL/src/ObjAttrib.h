#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

// foreward declaring the object class to avoid infinite inclusion
class Object;

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
	Transform(std::shared_ptr<glm::mat4> view, glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(0));
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
	std::shared_ptr<glm::mat4> view;

private:
};

class Mesh : public ObjAttrib
{
public:
	Mesh(const std::string& filepath, std::shared_ptr<VertexArray> vao);
	~Mesh();

	unsigned int Size();
	void OnImGuiRender() override;

private:
	void Parse();

public:
	std::shared_ptr<VertexArray> VAO;
	std::unique_ptr<VertexBuffer> VB;
	std::unique_ptr<VertexBufferLayout> VBL;
	std::unique_ptr<IndexBuffer> IB;
	std::string filepath;

private:
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
};

class Light : public ObjAttrib
{
public:
	Light(glm::vec4 lightdir, glm::vec4 lightcol, float lightint);
	~Light();

public:
	glm::vec4 lightDir;
	glm::vec4 lightCol;
	float lightInt;
};

class Material : public ObjAttrib
{
public:
	Material(const std::string& name, const std::string& shaderpath, 
		glm::vec4 diffuse, glm::vec4 ambient, float specint, glm::vec4 speccol);

	Material(const std::string& name, const std::string& shaderpath, const std::string& matpath, std::vector<std::shared_ptr<Object>>* lts);
	~Material();

	void OnUpdate() override;
	void OnImGuiRender() override;

private:
	void Parse(const std::string& matfile);

public:
	std::string name;
	std::string shaderPath;
	std::string materialPath;

	std::unique_ptr<Shader> shader;
	glm::vec4 diffuseCol;
	glm::vec4 ambientCol;
	float specInt;
	glm::vec4 specCol;
	std::vector<std::shared_ptr<Object>>* lights;
};

//class MeshRenderer : public ObjAttrib
//{
//public:
//	MeshRenderer(Mesh* m, Material* mat, const bool& islit=true);
//	~MeshRenderer();
//
//	void OnUpdate() override;
//	void OnImGuiRender() override;
//
//public:
//	Mesh* mesh;
//	Material* material;
//	bool isLit;
//};