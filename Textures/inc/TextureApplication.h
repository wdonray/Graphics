#pragma once
#include "Application.h"
#include <glm/glm.hpp>
#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"
struct Camera;
struct CameraApp;
struct Shader;
struct Mesh;
class TextureApplication : public Application
{
public:
	TextureApplication();
	~TextureApplication();
	Mesh* generateGrid(unsigned int rows, unsigned int cols);
	Camera * cam;
	CameraApp * camapp;
	Shader * shader;
	Mesh * plane;
	int m_rows;
	int m_cols;
	float runTime;
	unsigned m_textureID;
protected:
	bool startup() override;
	bool shutdown() override;
	bool update(float deltaTime) override;
	bool draw() override;
};

