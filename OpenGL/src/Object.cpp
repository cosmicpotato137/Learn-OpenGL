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

Object::Object(const std::string& name)
	: m_Name(name)
{
}

Object::~Object()
{
	//for (auto it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
		
}

void Object::OnUpdate()
{
	// iterate through attributes and update them
	for (auto it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
		it->second->OnUpdate();
}

void Object::Render(Renderer renderer, glm::mat4 proj, glm::mat4 view)
{
	Transform* transf = GetAttrib<Transform>();
	glm::mat4 modelview = view * transf->transform;

	Material* mat = GetAttrib<Material>();
	mat->shader->Bind();
	mat->shader->SetUniformMat4f("u_ModelView", modelview);
	mat->shader->SetUniformMat4f("u_Projection", proj);

	Mesh* mesh = GetAttrib<Mesh>();
	renderer.Draw(*mesh->VAO, *mesh->IB, *mat->shader);
}

void Object::OnImGuiRender()
{
	if (!ImGui::TreeNode(m_Name.c_str()))
		return;
	for (auto it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
		it->second->OnImGuiRender();
	ImGui::TreePop();
}

template <typename T>
bool Object::DelAttrib()
{
	std::string attribname = typeid(T).name();
	m_Attributes.erase(attribname);
	return true;
}

template <typename T>
T* Object::GetAttrib()
{
	std::string attribname = typeid(T).name();
	return (T*)&*m_Attributes[attribname];
}

void Object::SetAttrib(std::shared_ptr<ObjAttrib> attrib)
{
	std::string attribname = typeid(*attrib).name();
	m_Attributes[attribname] = attrib;
}
