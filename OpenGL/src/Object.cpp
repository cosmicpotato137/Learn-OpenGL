#include "Object.h"

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "Renderer.h"

//-----------------------------------
//
// Object
//
//-----------------------------------

Object::Object(Transform t)
	: m_Transf(t)
{
}

Object::~Object()
{
}

//-----------------------------------
//
// Solid Object - renderable
// 
//-----------------------------------

SolidObject::SolidObject(std::string& name, Transform transf, Mesh mesh, std::string& shaderpath)
	: m_Name(name), m_Transf(transf), m_Mesh(mesh)
{

	m_VAO = std::make_unique<VertexArray>();

	// vertex position buffer
	{
		m_VertBuff = std::make_unique<VertexBuffer>(&m_Mesh.vertices[0], m_Mesh.Size());
		VertexBufferLayout layout;
		layout.Push<glm::vec3>(2); // vertex positions
		m_VAO->AddBuffer(*m_VertBuff, layout);
	}

	m_IndexBuff = std::make_unique<IndexBuffer>(&m_Mesh.indices[0], m_Mesh.indices.size());

	m_Shader = std::make_unique<Shader>(shaderpath);

	m_VAO->Unbind();
	m_VertBuff->Unbind();
	m_IndexBuff->Unbind();
	m_Shader->Unbind();
	
}

SolidObject::~SolidObject()
{
}

void SolidObject::Render(Renderer renderer, glm::mat4 proj, glm::mat4 view)
{
	glm::mat4 modelview = view * m_Transf.transform;

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_ModelView", modelview);
	m_Shader->SetUniformMat4f("u_Projection", proj);
	renderer.Draw(*m_VAO, *m_IndexBuff, *m_Shader);
}

void SolidObject::Update()
{
	m_Transf.UpdateTransform();
}
