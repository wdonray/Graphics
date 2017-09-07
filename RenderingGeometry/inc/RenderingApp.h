#pragma once
#include "Application.h"
#include <glm/glm.hpp>
struct Camera;
struct LoadFile;
struct CameraApp;
struct Mesh;
struct Shader;

class RenderingApp :
	public Application
{
public:
	RenderingApp();
	~RenderingApp();

	void generateGrid(unsigned int rows, unsigned int cols);
	Mesh* genreateSphere(float radius, float verts) const;
	// our vertex and index buffers
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_programID;
	Camera * cam;
	LoadFile * fl;
	CameraApp * camapp;
	Mesh * mesh;
	Mesh * sphere;
	Shader * shader;
	const char * vsSource;
	const char * fsSource;
	unsigned int m_rows;
	unsigned int m_cols;
	float runTime;
	glm::mat4 rotationView;

protected:
	bool startup() override;
	bool shutdown() override;
	bool update(float deltaTime) override;
	bool draw() override;
};

