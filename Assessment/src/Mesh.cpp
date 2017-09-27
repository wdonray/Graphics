#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::Mesh() : index_count(0), vertex_count(0), vertRef(m_vertices), m_VAO(0), m_VBO(0), m_IBO(0)
{
}

Mesh::~Mesh()
{
}
void Mesh::initialize(vector<Vertex>& verts, vector<unsigned>& indices)
{
	for (auto v : verts)
		m_vertices.push_back(v);
	for (auto i : indices)
		m_indices.push_back(i);
	index_count = m_indices.size();
	vertex_count = m_vertices.size();
}

void Mesh::Create_Buffers()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
		m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
		m_indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3 + sizeof(glm::vec2)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::bind()
{
	glBindVertexArray(m_VAO);
}

void Mesh::unbind()
{
	glBindVertexArray(0);
}

bool Mesh::draw(unsigned int drawMode) const
{
	switch (drawMode)
	{
	case GL_LINES:
		glDrawElements(GL_LINES, this->index_count, GL_UNSIGNED_INT, nullptr);
		break;

	case GL_TRIANGLES:
		glDrawElements(GL_TRIANGLES, this->index_count, GL_UNSIGNED_INT, nullptr);
		break;

	case GL_TRIANGLE_STRIP:
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xFFFF);
		glDrawElements(GL_TRIANGLE_STRIP, this->index_count, GL_UNSIGNED_INT, nullptr);
		glDisable(GL_PRIMITIVE_RESTART);
		break;

	case GL_POINTS:
		glDrawElements(GL_POINTS, 0, this->vertRef.size(), nullptr);
		break;

	default: 
		return false;
	}
	return true;
}
