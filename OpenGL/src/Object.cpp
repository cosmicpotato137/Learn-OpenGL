#include "Object.h"

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "Renderer.h"
#include "imgui/imgui.h"

//-----------------------------------
//
// Object
//
//-----------------------------------

Object::Object(const std::string& name, const Transform& t)
	: m_Name(name)
{
	m_Transf = std::make_unique<Transform>(t);
}

Object::~Object()
{
}

//-----------------------------------
//
// Scene Object
// 
//-----------------------------------

SceneObject::SceneObject(const std::string& name, const Transform& transf, std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat)
	: Object(name, transf), m_Mesh(mesh), m_Material(mat)
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::OnUpdate()
{
	m_Material->OnUpdate();
	m_Transf->OnUpdate();
}

void SceneObject::Render(Renderer renderer, glm::mat4 proj, glm::mat4 view)
{
	glm::mat4 modelview = view * m_Transf->transform;

	m_Material->shader->Bind();
	m_Material->shader->SetUniformMat4f("u_ModelView", modelview);
	m_Material->shader->SetUniformMat4f("u_Projection", proj);
}

void SceneObject::OnImGuiRender()
{
	if (!ImGui::TreeNode(m_Name.c_str()))
		return;
	m_Transf->OnImGuiRender();
	m_Mesh->OnImGuiRender();
	m_Material->OnImGuiRender();
	ImGui::TreePop();
}

void SceneObject::AddAttribute(const ObjAttrib& attrib)
{
	m_Attributes->push_back(attrib);
}
