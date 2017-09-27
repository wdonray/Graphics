#include "TextureApplication.h"
#include "CameraApp.h"
#include <gl_core_4_4.h>
#include "Camera.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <Mesh.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


TextureApplication::TextureApplication() : m_rows(0), m_cols(0), runTime(0), m_textureID(0)
{
	cam = new Camera();
	camapp = new CameraApp();
	shader = new Shader();
	plane = new Mesh();
}


TextureApplication::~TextureApplication()
{
	delete camapp;
	delete cam;
	delete shader;
	delete plane;
}

Mesh* TextureApplication::generateGrid(unsigned int rows, unsigned int cols)
{
	vector<vec2> uvs;
	auto aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols ; ++c)
		{
			uvs.push_back(vec2((float)r / (float)rows, (float)c / (float)cols));
			Vertex verts = {
				vec4(float(c), 0, float(r), 1),
				vec4(sin(r), cos(c), 0, 1),
				vec4(0, 1, 0, 0),
				vec2((float)r / (float)rows, (float)c / (float)cols)
			};
			aoVertices[r * cols + c] = verts;
		}
	}

	vector<Vertex> verts = vector<Vertex>();
	vector<unsigned int> indices = vector<unsigned int>();

	//Defining index count based off quad count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			//Triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			//Triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}
	//Create and bind buffers to a vertex array object
	m_rows = rows;
	m_cols = cols;

	for (unsigned int i = 0; i < (rows * cols); i++)
		verts.push_back(aoVertices[i]);
	for (unsigned int i = 0; i < index; i++)
		indices.push_back(auiIndices[i]);

	plane->initialize(verts, indices);
	plane->bind();

	delete[] aoVertices;
	delete[] auiIndices;
	return plane;
}

bool TextureApplication::startup()
{
	setBackgroundColor(1.0f, 1.0f, 1.0f, 1);
	cam->setLookAt(cam->m_posvec3, vec3(0), vec3(0, 1, 0));

	shader->load("textured.vert", GL_VERTEX_SHADER, true);
	shader->load("textured.frag", GL_FRAGMENT_SHADER, true);
	shader->attach();

	m_rows = 5 , m_cols = 5;
	plane = generateGrid(m_rows, m_cols);
	plane->Create_Buffers();

	int imageWidth, imageHeight, imageFormat;
	unsigned char* data = stbi_load("./textures/crate.png", &imageWidth, &imageHeight, &imageFormat, STBI_default);
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

	return false;
}

bool TextureApplication::shutdown()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) || m_GameOver)
		glfwSetWindowShouldClose(m_window, true);
	return false;
}

bool TextureApplication::update(float deltaTime)
{
	runTime += deltaTime;
	camapp->Keyboard_Movement(cam, m_window);
	camapp->Mouse_Movement(cam, m_window);
	return false;
}

bool TextureApplication::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	unsigned int projectionViewUniform = shader->getUniform("ProjectionViewModel");
	unsigned int time = shader->getUniform("time");
	unsigned int crate = shader->getUniform("textcrate");

	shader->bind();

	mat4 pvm = cam->getProjectionView();
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, &pvm[0][0]);
	glUniform1f(time, glfwGetTime());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	//draw
	plane->bind();
	glUniform1i(crate, 0);
	plane->draw(GL_TRIANGLES);
	plane->unbind();

	shader->unbind();
	return false;
}
