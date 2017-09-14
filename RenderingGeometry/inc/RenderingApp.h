#pragma once
#include "Application.h"
#include <glm/glm.hpp>
#include <vector>
#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"
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

	Mesh* generateGrid(unsigned int rows, unsigned int cols);
	std::vector<glm::vec4> generateHalfCircle(float radius, float numPoints);
	std::vector<glm::vec4> rotatePoints(std::vector<glm::vec4> points, float numMeridians);
	std::vector<unsigned int> genIndices(unsigned int nm, unsigned int np);
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
	Mesh * gridMesh;
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

