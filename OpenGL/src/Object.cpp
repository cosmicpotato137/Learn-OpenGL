#include "Object.h"

#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "Renderer.h"

Object::Object(std::string name, std::string filepath, std::string shaderpath)
	: m_Name(name), m_Filepath(filepath),
	m_Transform(glm::mat4(1.0f)), m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
	m_Scale(1.0f, 1.0f, 1.0f)
{
	parse();

	m_VAO = std::make_unique<VertexArray>();

	// vertex position buffer
	{
		m_VertBuff = std::make_unique<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(glm::vec3));
		VertexBufferLayout layout;
		layout.Push<glm::vec3>(1); // vertex positions
		m_VAO->AddBuffer(*m_VertBuff, layout);
	}

	// normal buffer
	{
		m_NormalBuff = std::make_unique<VertexBuffer>(&m_Normals[0], m_Normals.size() * sizeof(glm::vec3));
		VertexBufferLayout layout;
		layout.Push<glm::vec3>(1);
		m_VAO->AddBuffer(*m_NormalBuff, layout, 1);
	}

	m_IndexBuff = std::make_unique<IndexBuffer>(&m_Indices[0], m_Indices.size());

	m_Shader = std::make_unique<Shader>(shaderpath);

	m_VAO->Unbind();
	m_VertBuff->Unbind();
	m_IndexBuff->Unbind();
	m_Shader->Unbind();
	
}

Object::~Object()
{
}

void Object::Render(Renderer renderer, glm::mat4 proj, glm::mat4 view)
{
	glm::mat4 modelview = view * m_Transform;

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_ModelView", modelview);
	m_Shader->SetUniformMat4f("u_Projection", proj);
	renderer.Draw(*m_VAO, *m_IndexBuff, *m_Shader);
}

void Object::Update()
{
	glm::mat4 sca = glm::scale(glm::mat4(1.0f), m_Scale);
	glm::mat4 pos = glm::translate(glm::mat4(1.0f), m_Position);

	m_Transform = sca * pos;
}

void Object::parse()
{
	FILE* fp;
	float x, y, z;
	int fa, fb, fc, ignore;
	char c1, c2;
	float minY = INFINITY, minZ = INFINITY;
	float maxY = -INFINITY, maxZ = -INFINITY;

	fopen_s(&fp, m_Filepath.c_str(), "rb");

	if (fp == NULL) {
		std::cerr << "Error loading file: " << m_Filepath << std::endl;
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
			m_Vertices.push_back(glm::vec3(x, y, z));
			if (y < minY) minY = y;
			if (z < minZ) minZ = z;
			if (y > maxY) maxY = y;
			if (z > maxZ) maxZ = z;
		}
		// get normals
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf_s(fp, "%f %f %f", &x, &y, &z);
			m_Normals.push_back(glm::normalize(glm::vec3(x, y, z)));
		}
		else if (c1 == 'f' && c2 == ' ')
		{
			fscanf_s(fp, "%d//%d %d//%d %d//%d", 
				&fa, &ignore, &fb, &ignore, 
				&fc, &ignore);
			// triangle 1
			m_Indices.push_back(fa - 1);
			m_Indices.push_back(fb - 1);
			m_Indices.push_back(fc - 1);
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
}
