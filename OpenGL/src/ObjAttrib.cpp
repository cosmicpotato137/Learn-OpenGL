#include "ObjAttrib.h"

//-----------------------------------
//
// Transform
//
//-----------------------------------

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
	: position(pos), rotation(rot), scale(scale)
{
}

Transform::~Transform()
{
}

glm::vec3 Transform::ApplyTransf(glm::vec4 vec)
{
	UpdateTransform();
	return transform * vec;
}

void Transform::ApplyTransfInpl(glm::vec4& vec)
{
	UpdateTransform();
	vec = transform * vec;
}

glm::vec3 Transform::operator*(glm::vec4 vec)
{
	return ApplyTransf(vec);
}

//glm::mat4 Transform::operator*(glm::mat4 mat4)
//{
//	return 
//}

//-----------------------------------
//
// Mesh
//
//-----------------------------------

Mesh::Mesh(const char* filepath)
	: m_Filepath(filepath)
{
	Parse();
}

Mesh::~Mesh()
{
}

unsigned int Mesh::Size()
{
	return vertices.size() * sizeof(glm::vec3);
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
			fscanf_s(fp, "%d//%d %d//%d %d//%d", &fa, &na, &fb, &nb, &fc, &nc);
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

	for (int i = 0; i < indices.size(); i++)
	{
		vertices.push_back(verts[inds[i].x]);
		vertices.push_back(norms[inds[i].y]);
		indices.push_back(i);
	}
}
