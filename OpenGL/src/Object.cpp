#include "Object.h"
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

void Object::OnRender()
{
	MeshRenderer renderer = *GetAttrib<MeshRenderer>();
	renderer.Draw(*GetAttrib<Transform>(), *GetAttrib<Mesh>());
}

void Object::OnImGuiRender()
{
	if (!ImGui::TreeNode(m_Name.c_str()))
		return;
	for (auto it = m_Attributes.begin(); it != m_Attributes.end(); ++it)
		it->second->OnImGuiRender();
	ImGui::TreePop();
}

void Object::SetAttrib(std::shared_ptr<ObjAttrib> attrib)
{
	std::string attribname = typeid(*attrib).name();
	m_Attributes[attribname] = attrib;
}