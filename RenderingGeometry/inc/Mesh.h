#pragma once
#include <vector>
#include "RenderingApp.h"
using namespace  std;
class Mesh
{
public:
	Mesh();
	~Mesh();
	void Create_Buffers();
	void initialize(vector<RenderingApp::Vertex>& verts, vector<unsigned int>& indeces);
	void bind();
	void unbind();
	unsigned int index_count;
	unsigned int vertex_count;
private:
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
	vector<RenderingApp::Vertex> m_indeces;
	vector<unsigned int> m_vertices;
};

