#pragma once
#include <Application.h>
#include <glm/glm.hpp>
#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"

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
	void generateSphere(unsigned segments, unsigned rings, unsigned& vao, unsigned& vbo, unsigned& ibo, unsigned& indexCount) const;
	static int TextEditCallBackStub(ImGuiTextEditCallbackData* data);
	static int TextEditCallback(ImGuiTextEditCallbackData* data);
	Mesh* generateGrid(unsigned int rows, unsigned int cols);
	Camera * cam;
	CameraApp * camapp;
	Shader * shader;
	Mesh * gridMesh;
	float runTime, rotateSpeed = 2.f;
	bool rotate = true, fill = true;
	int m_rows, m_cols;
	unsigned char* data;
	void loadTexture();
	ImVec4 ball_color = ImColor(0, 0, 0), clear_color = ImColor(114, 144, 154);
private:
	void onGUI();

	struct toGenSphere
	{
		unsigned segments;
		unsigned rings;
		unsigned int index_count;
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_IBO;
		int scaleAmt;
		ImVec4 spherePos;
	} m_Sphere;

	struct ImageInformation
	{
		int imageWidth;
		int imageHeight;
		int imageFormat;
		unsigned m_textureID;
	} m_image;

	struct DirectionalLight
	{
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	} m_directLight;

	glm::vec3 m_ambientLight;

	struct Material
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float specularPower;
	} m_material;

	glm::mat4 m_modelMatrix;

	struct ShaderData
	{
		Shader* shader;
		char* source;
		unsigned type;
		bool isFile;
	};
protected:
	bool startup() override;
	bool shutdown() override;
	bool update(float deltaTime) override;
	bool draw() override;
};

