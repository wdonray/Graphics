#pragma once
#include "Application.h"
#include <glm/glm.hpp>
#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <gl_core_4_4.h>
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
	int imageWidth, imageHeight, imageFormat;
	unsigned char* data;
	bool switchTest = false;
	float runTime;
	unsigned m_textureID;
	char * textureLoad;
	unsigned m_perlinTexture;
	void PerlinTest();
	static double DonrayNoise(glm::vec2);
	static glm::vec2 InterpolatedNoise(glm::vec2, int);
private:
	void OnGUI() const;
protected:
	bool startup() override;
	bool shutdown() override;
	bool update(float deltaTime) override;
	bool draw() override;
};

