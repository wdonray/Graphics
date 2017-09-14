#pragma once
#include <Application.h>

struct Camera;
struct CameraApp;
struct Shader;
struct Mesh;
class LightingApp :
	public Application
{
public:
	LightingApp();
	~LightingApp();
	void generateSphere(unsigned segments, unsigned rings, unsigned& vao, unsigned& vbo, unsigned& ibo, unsigned& indexCount);
	Camera * cam;
	CameraApp * camapp;
	Shader * shader;
	float runTime;
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int index_count;
protected:
	bool startup() override;
	bool shutdown() override;
	bool update(float deltaTime) override;
	bool draw() override;
};

