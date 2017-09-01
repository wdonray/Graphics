#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::Mesh() : index_count(0), vertex_count(0), m_VAO(0), m_VBO(0), m_IBO(0)
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
}

void Mesh::bind()
{
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
		m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),
		m_indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));

}

void Mesh::unbind()
{
	glBindVertexArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
