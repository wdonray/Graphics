#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 normal;
	glm::vec2 texcoord;
	glm::vec4 tangent;
	glm::vec4 bitangent;
};
using namespace  std;
class Mesh
{
public:
	Mesh();
	~Mesh();
	void Create_Buffers();
	void initialize(vector<Vertex>& verts, vector<unsigned int>& indeces);
	void bind();
	void unbind();
	bool draw(unsigned int) const;
	unsigned int index_count;
	unsigned int vertex_count;
	std::vector<Vertex>& vertRef;
private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	vector<unsigned int> m_indices;
	vector<Vertex> m_vertices;
};

