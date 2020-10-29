#include "CameraMove.h"
#include "imgui/imgui.h"

CameraMove::CameraMove(std::shared_ptr<Transform> transf, float rotspd)
	: transf(transf), mousex(0), mousey(0), rotSpeed(rotspd)
{
}

CameraMove::~CameraMove()
{
}

void CameraMove::OnImGuiRender()
{
	if (!ImGui::TreeNode("CameraMove"))
		return;
	ImGui::InputFloat("Rotation Speed", &rotSpeed);
	ImGui::TreePop();
}

void CameraMove::MouseUpdate(bool mousedown, float xpos, float ypos)
{
	float mousedx = mousex - xpos;
	float mousedy = mousey - ypos;
	mousex = xpos;
	mousey = ypos;
	float angle;
	float dist = glm::length(transf->position);
	// calculate new transform
	if (mousedown)
	{
		angle = atan(mousedx / dist) * glm::radians(rotSpeed) * dist;
		glm::mat4 rot = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
		transf->UpdatePosition(rot);
		transf->UpdateBasis(rot);

		angle = atan(mousedy / dist) * glm::radians(rotSpeed) * dist;
		glm::vec3 piv = glm::normalize(glm::cross(transf->up, transf->forward));
		rot = glm::rotate(glm::mat4(1.0f), -angle, piv);
		transf->UpdatePosition(rot);
		transf->UpdateBasis(rot);

	}
}

void CameraMove::ScrollUpdate(float xoffset, float yoffset)
{
	glm::vec3& eye = transf->position;
	float amt = glm::sqrt(glm::length(eye)) * 4;
	glm::vec3 en = glm::normalize(eye);

	eye += en * amt * (float)yoffset;
}
