#include "Mesh.h"

Mesh::Mesh(): index_count(0), vertex_count(0), m_vao(0), m_vbo(0), m_ibo(0)
{
}

Mesh::~Mesh()
{
}

void Mesh::Create_Buffers()
{
}

void Mesh::initialize(vector<RenderingApp::Vertex>& verts, vector<unsigned>& indeces)
{
}

void Mesh::bind()
{
}

void Mesh::unbind()
{
}
