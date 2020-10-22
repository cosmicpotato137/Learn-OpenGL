#pragma once

#include <vector>
#include <string>

#include "Shader.h"
#include "VertexArray.h"
#include "BufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "BufferLayout.h"
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
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::mat4 transform;
public:
	Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(0));
	~Transform();

	void OnUpdate() override;
	void OnImGuiRender() override;

	void UpdateTransform();

};

class Mesh : public ObjAttrib
{
public:
	std::shared_ptr<VertexArray> VAO;
	std::unique_ptr<IndexBuffer> IB;

private:
	std::string filepath;
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;
	std::unique_ptr<VertexBuffer> VB;

public:
	Mesh(const std::string& filepath, std::shared_ptr<VertexArray> vao);
	~Mesh();

	unsigned int Size();
	void OnImGuiRender() override;

private:
	void Parse();
};

class Camera : public ObjAttrib
{
public:
	bool active;
	glm::mat4 projection;
	glm::mat4 view;
private:
	std::shared_ptr<Transform> transf;
	glm::vec3 eye, center, up;
public:
	Camera(glm::mat4 proj, std::shared_ptr<Transform> transf);
	~Camera();

	void OnUpdate() override;

	void UpdateView();
};

class Light : public ObjAttrib
{
public:
	bool active;
	glm::vec4 lightDir;
	glm::vec4 lightCol;
	float lightInt;

public:
	Light(glm::vec4 lightdir, glm::vec4 lightcol, float lightint);
	~Light();

	void OnUpdate() override;
	void OnImGuiRender() override;
};

class Material : public ObjAttrib
{
public:
	std::string name;
	std::string shaderPath;
	std::string materialPath;

	std::shared_ptr<Shader> shader;
	glm::vec4 diffuseCol;
	glm::vec4 ambientCol;
	float specInt;
	glm::vec4 specCol;
	
	std::shared_ptr<UniformBuffer> lightBuffer;

public:
	Material(const std::string& name, const std::string& shaderpath, const std::string& matpath, std::shared_ptr<UniformBuffer> lightbuffer);
	~Material();

	void OnUpdate() override;
	void OnImGuiRender() override;

private:
	void Parse(const std::string& matfile);
};

class MeshRenderer : public ObjAttrib
{
private:
	std::shared_ptr<Material> material;
	Object* activeCamera;
	bool isLit;

public:
	MeshRenderer(std::shared_ptr<Material> mat, Object* activecamera);
	~MeshRenderer();

	void OnUpdate() override;
	void OnImGuiRender() override;

	void Clear() const;
	void Draw(const Transform& transf, const Mesh& mesh);
};