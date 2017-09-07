#pragma once
#include <vector>
#include <glm/detail/type_vec4.hpp>

struct Vertex
{
	glm::vec4 position, color;
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

