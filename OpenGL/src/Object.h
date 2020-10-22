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
	
	template <typename T> bool DelAttrib();
	template <typename T> std::shared_ptr<T> GetAttrib();
	void SetAttrib(std::shared_ptr<ObjAttrib> attrib);
};