#pragma once

#include <vector>
#include <unordered_map>

#include "ObjAttrib.h"

class Object
{
public:
	std::string m_Name;
private:
	std::unordered_map<std::string, std::shared_ptr<ObjAttrib>> m_Attributes;

public:
	Object(const std::string& name);
	~Object();

	void OnUpdate();
	void OnRender();
	void OnImGuiRender();
	
	template <typename T> bool DelAttrib()
	{
		std::string attribname = typeid(T).name();
		m_Attributes.erase(attribname);
		return true;
	}

	template <typename T> std::shared_ptr<T> GetAttrib()
	{
		std::string attribname = typeid(T).name();
		return std::dynamic_pointer_cast<T>(m_Attributes[attribname]);
	}

	void SetAttrib(std::shared_ptr<ObjAttrib> attrib);
};