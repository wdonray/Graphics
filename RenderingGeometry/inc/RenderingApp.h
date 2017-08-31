#pragma once
#include "Application.h"
#include <glm/detail/type_vec4.hpp>
#include <Camera.h>
#include <Shader.h>
class RenderingApp :
	public Application
{
public:
	RenderingApp();
	~RenderingApp();
	struct Vertex
	{
		glm::vec4 position, color;
	};
	void generateGrid(unsigned int rows, unsigned int cols);
	// our vertex and index buffers
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_programID;
	Camera * cam;
	Shader * fl;
	const char * vsSource;
	const char * fsSource;
	unsigned int m_rows;
	unsigned int m_cols;
	bool startup() override;
	bool shutdown() override;
	bool update(float deltaTime) override;
	bool draw() override;
};

