#include "ObjAttrib.h"
#include "Object.h"
#include "imgui/imgui.h"

//-----------------------------------
//
// Transform
//
//-----------------------------------

Transform::Transform(std::shared_ptr<glm::mat4> view, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
	: view(view), position(pos), rotation(rot), scale(scale)
{
	OnUpdate();
}

Transform::~Transform()
{
}

glm::vec3 Transform::ApplyTransf(glm::vec4 vec)
{
	OnUpdate();
	return transform * vec;
}

void Transform::ApplyTransfInpl(glm::vec4& vec)
{
	OnUpdate();
	vec = transform * vec;
}

void Transform::OnUpdate()
{
	glm::mat4 sca = glm::scale(glm::mat4(1), scale);
	glm::mat4 pos = glm::translate(glm::mat4(1), position);

	transform = sca * pos;
}

void Transform::OnImGuiRender()
{
	if (!ImGui::TreeNode("Transform"))
		return;
	ImGui::InputFloat3("Position", &position[0], 3);
	ImGui::InputFloat3("Rotation", &rotation[0], 3);
	ImGui::InputFloat3("Scale", &scale[0], 3);
	ImGui::TreePop();
}

//-----------------------------------
//
// Light
//
//-----------------------------------

Light::Light(glm::vec4 lightdir, glm::vec4 lightcol, float lightint)
	: lightDir(lightdir), lightCol(lightcol), lightInt(lightint)
{
}

Light::~Light()
{
}

void Light::OnImGuiRender()
{
	if (!ImGui::TreeNode("Direction Light"))
		return;
	ImGui::InputFloat3("Direction", &lightDir[0]);
	ImGui::ColorEdit4("Color", &lightCol[0]);
	ImGui::SliderFloat("Intensity", &lightInt, 0, 10);
	ImGui::TreePop();
}

//-----------------------------------
//
// Mesh
//
//-----------------------------------

Mesh::Mesh(const std::string& fp, std::shared_ptr<VertexArray> vao)
	: filepath(fp), VAO(vao)
{
	Parse();

	VB = std::make_unique<VertexBuffer>(&vertices[0], Size());
	VBL = std::make_unique<VertexBufferLayout>();
	VBL->Push<glm::vec3>(2); // vertex positions
	vao->AddBuffer(*VB, *VBL);

	IB = std::make_unique<IndexBuffer>(&indices[0], indices.size());
}

Mesh::~Mesh()
{
}

unsigned int Mesh::Size()
{
	return vertices.size() * sizeof(glm::vec3);
}

void Mesh::OnImGuiRender()
{
	if (!ImGui::TreeNode("Mesh"))
		return;
	std::string file = std::string("File: ") + filepath;
	ImGui::Text(file.c_str());
	ImGui::Text("Vertices: %i", vertices.size());
	ImGui::TreePop();
}

void Mesh::Parse()
{
	FILE* fp;
	float x, y, z;
	int fa, fb, fc, na, nb, nc, ignore;
	char c1, c2;
	float minY = INFINITY, minZ = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY;

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> norms;
	std::vector<glm::vec2> inds;

	fopen_s(&fp, filepath.c_str(), "rb");

	if (fp == NULL) {
		std::cerr << "Error loading file: " << filepath << std::endl;
		std::getchar();
		exit(-1);
	}

	// feof returns end of file
	while (!feof(fp)) {
		c1 = fgetc(fp); // returns character as int
		while (!(c1 == 'v' || c1 == 'f')) {
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);

		// get vertices
		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			verts.push_back(glm::vec3(x, y, z));
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		// get normals
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			norms.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if (c1 == 'f' && c2 == ' ')
		{
			fscanf_s(fp, "%d/%d/%d %d/%d/%d %d/%d/%d", &fa, &na, &ignore, &fb, &nb, &ignore, &fc, &nc, &ignore);
			// triangle indices -- (vert, normal)
			inds.push_back(glm::vec2(fa - 1, na - 1));
			inds.push_back(glm::vec2(fb - 1, nb - 1));
			inds.push_back(glm::vec2(fc - 1, nc - 1));
		}
		else if (c1 == 's' && c2 == ' ')
		{
		}
	}

	fclose(fp); // Finished parsing

	// Recenter the teapot
	float avgY = (minY + maxY) / 2.0f - 0.0234f;
	float avgZ = (minZ + maxZ) / 2.0f;
	//for (unsigned int i = 0; i < m_Vertices.size(); ++i) {
	//	glm::vec3 shiftedVertex = (m_Vertices[i] - glm::vec3(0.0f, avgY, avgZ)) * glm::vec3(0.975f, 0.975f, 0.975f);
	//	m_Vertices[i] = shiftedVertex;
	//}

	for (unsigned int i = 0; i < inds.size(); i++)
	{
		vertices.push_back(verts[inds[i].x]);
		vertices.push_back(norms[inds[i].y]);
		indices.push_back(i);
	}
}

//-----------------------------------
//
// Material
//
//-----------------------------------

Material::Material(const std::string& name, const std::string& shaderpath, const std::string& matpath, std::shared_ptr<UniformBuffer> lightbuffer)
	: name(name), shaderPath(shaderpath), materialPath(matpath), lightBuffer(lightbuffer)
{
	Parse(matpath);
	shader = std::make_shared<Shader>(shaderpath);
	lightBuffer->BindUniformBlock("Light", shader);
	LOG(name);
	shader->PrintUniforms();
	OnUpdate();
}

Material::~Material()
{
}

void Material::OnUpdate()
{
	shader->Bind();
	shader->SetUniform4fv("u_Ambient", &ambientCol[0]);
	shader->SetUniform4fv("u_Diffuse", &diffuseCol[0]);
	shader->SetUniform4fv("u_Specular", &specCol[0]);
	shader->SetUniform1f("u_SpecInt", specInt);
}

void Material::OnImGuiRender()
{
	if (!ImGui::TreeNode("Material"))
		return;
	ImGui::Text(shaderPath.c_str());
	ImGui::ColorEdit4("Diffuse Color", &diffuseCol[0]);
	ImGui::ColorEdit4("Ambient Color", &ambientCol[0]);
	ImGui::ColorEdit4("Specular Color", &specCol[0]);
	ImGui::SliderFloat("Specular Intensity", &specInt, 0.0f, 500.0f);
	ImGui::TreePop();
}

void Material::Parse(const std::string& matfile)
{
	FILE* fp;
	float hi;
	float va, vb, vc, ignore;
	char c1, c2;

	fopen_s(&fp, matfile.c_str(), "rb");

	if (fp == NULL) {
		std::cerr << "Error loading file: " << matfile << std::endl;
		std::getchar();
		exit(-1);
	}

	// feof returns end of file
	while (!feof(fp)) {
		c1 = fgetc(fp); // returns character as int
		while (!(c1 == 'K' || c1 == 'N' || c1 == 'd' || c1 == 'i')) {
			c1 = fgetc(fp);
			if (feof(fp))
				break;
		}
		c2 = fgetc(fp);

		// get vertices
		if ((c1 == 'N') && (c2 == 's')) {
			fscanf_s(fp, " %f", &hi);
			specInt = hi;
		}
		// get normals
		else if ((c1 == 'K') && (c2 == 'a')) {
			fscanf_s(fp, " %f %f %f", &va, &vb, &vc);
			ambientCol = glm::vec4(va, vb, vc, 1.0f);
		}
		else if (c1 == 'K' && c2 == 'd')
		{
			fscanf_s(fp, " %f %f %f", &va, &vb, &vc);
			diffuseCol = glm::vec4(va, vb, vc, 1.0f);
		}
		else if (c1 == 'K' && c2 == 's')
		{
			fscanf_s(fp, " %f %f %f", &va, &vb, &vc);
			specCol = glm::vec4(va, vb, vc, 1.0f);
		}
	}
}

//-----------------------------------
//
// Mesh Renderer
//
//-----------------------------------

MeshRenderer::MeshRenderer(std::shared_ptr<Material> mat, std::shared_ptr<glm::mat4> proj)
	: material(mat), proj(proj), isLit(true)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::OnUpdate()
{

}

void MeshRenderer::OnImGuiRender()
{
	if (!ImGui::TreeNode("Mesh Renderer"))
		return;
	std::string namestr = std::string("Material: ") + material->name;
	ImGui::Text(namestr.c_str());
	ImGui::Checkbox("Lighting", &isLit);

	ImGui::TreePop();
}

void MeshRenderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void MeshRenderer::Draw(const Transform& transf, const Mesh& mesh)
{
	glm::mat4 modelview = *transf.view * transf.transform;

	material->shader->Bind();
	material->shader->SetUniformMat4f("u_ModelView", modelview);
	material->shader->SetUniformMat4f("u_Projection", *proj);

	material->shader->Bind();
	mesh.VAO->Bind();
	mesh.IB->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, mesh.IB->GetCount(), GL_UNSIGNED_INT, nullptr));
}