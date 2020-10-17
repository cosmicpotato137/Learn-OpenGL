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

void Object::Render(Renderer renderer, glm::mat4 proj)
{
	std::shared_ptr<Transform> transf = GetAttrib<Transform>();
	std::shared_ptr<Material> mat = GetAttrib<Material>();

	mat->shader->Bind();
	mat->shader->SetUniformMat4f("u_Model", transf->transform);
	mat->shader->SetUniformMat4f("u_View", *transf->view);
	mat->shader->SetUniformMat4f("u_Projection", proj);

	std::shared_ptr<Mesh> mesh = GetAttrib<Mesh>();
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

// define all uses of the template funciton for compiler
template bool Object::DelAttrib<Mesh>();
template bool Object::DelAttrib<Transform>();
template bool Object::DelAttrib<Material>();
template bool Object::DelAttrib<Light>();
template <typename T>
bool Object::DelAttrib()
{
	std::string attribname = typeid(T).name();
	m_Attributes.erase(attribname);
	return true;
}

template std::shared_ptr<Mesh> Object::GetAttrib<Mesh>();
template std::shared_ptr<Transform> Object::GetAttrib<Transform>();
template std::shared_ptr<Material> Object::GetAttrib<Material>();
template std::shared_ptr<Light> Object::GetAttrib<Light>();
template <typename T>
std::shared_ptr<T> Object::GetAttrib()
{
	std::string attribname = typeid(T).name();
	return std::dynamic_pointer_cast<T>(m_Attributes[attribname]);
}

void Object::SetAttrib(std::shared_ptr<ObjAttrib> attrib)
{
	std::string attribname = typeid(*attrib).name();
	m_Attributes[attribname] = attrib;
}
