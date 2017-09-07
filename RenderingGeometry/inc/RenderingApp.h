#pragma once
#include "Application.h"
#include <glm/glm.hpp>
#include <vector>
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

	//void generateGrid(unsigned int rows, unsigned int cols);
	Mesh* generateHalfCircle(float radius, float numPoints);
	Mesh* rotatePoints(std::vector<glm::vec4> points, float numMeridians);
	Mesh* generateCube();

	// our vertex and index buffers
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_programID;
	Camera * cam;
	LoadFile * fl;
	CameraApp * camapp;
	Mesh * cubeMesh;
	Mesh * sphereMesh;
	Shader * shader;
	const char * vsSource;
	const char * fsSource;
	unsigned int m_rows;
	unsigned int m_cols;
	float runTime;
	float slice;
	float theta;
	glm::mat4 rotationView;

protected:
	bool startup() override;
	bool shutdown() override;
	bool update(float deltaTime) override;
	bool draw() override;
};

