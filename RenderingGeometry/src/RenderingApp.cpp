#include "RenderingApp.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <Camera.h>
#include <Shader.h>
#include <CameraApp.h>
#include "Mesh.h"


RenderingApp::RenderingApp() : m_VAO(0), m_VBO(0), m_IBO(0), m_programID(0),
cam(nullptr), fl(nullptr), vsSource(nullptr),
fsSource(nullptr), m_rows(0), m_cols(0), runTime(0)
{
	cam = new Camera();
	fl = new Shader();
	camapp = new CameraApp();
	mesh = new Mesh();
	vsSource = fl->load("vsSource.vert");
	fsSource = fl->load("fsSource.vert");
	rotationView = mat4
	(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-5, 0, -5, 1
	);
}

RenderingApp::~RenderingApp()
{
}

//void RenderingApp::generateGrid(unsigned int rows, unsigned int cols)
//{
//	auto aoVertices = new Vertex[rows * cols];
//	for (unsigned int r = 0; r < rows; ++r)
//	{
//		for (unsigned int c = 0; c < cols; ++c)
//		{
//			aoVertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);
//			//Create some arbitrary color based off something
//			//that might not be related to tiling a tecture
//			vec3 colour = vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));
//			aoVertices[r * cols + c].color = vec4(colour, 1);
//		}
//	}
//	//Defining index count based off quad count (2 triangles per quad)
//	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
//	unsigned int index = 0;
//	for (unsigned int r = 0; r < (rows - 1); ++r)
//	{
//		for (unsigned int c = 0; c < (cols - 1); ++c)
//		{
//			//Triangle 1
//			auiIndices[index++] = r * cols + c;
//			auiIndices[index++] = (r + 1) * cols + c;
//			auiIndices[index++] = (r + 1) * cols + (c + 1);
//			//Triangle 2
//			auiIndices[index++] = r * cols + c;
//			auiIndices[index++] = (r + 1) * cols + (c + 1);
//			auiIndices[index++] = r * cols + (c + 1);
//		}
//	}
//
//
//
//	//Create and bind buffers to a vertex array object
//
//	m_rows = rows;
//	m_cols = cols;
//
//	delete[] aoVertices;
//	delete[] auiIndices;
//}

bool RenderingApp::startup()
{
	setBackgroundColor(1, 1, 1, 1.0f);
	cam->setLookAt(vec3(10, 10, 10), vec3(1, 3, 1), vec3(0, 1, 0));
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, nullptr);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, nullptr);
	glCompileShader(fragmentShader);
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, nullptr, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Vertex a0 = { vec4(0, 0, 0, 1), vec4(1, 1, 1, 1) };
	Vertex b1 = { vec4(3, 0, 0, 1), vec4(1, 1, 1, 1) };
	Vertex c2 = { vec4(0, 0, -3, 1), vec4(1, 1, 1, 1) };
	Vertex d3 = { vec4(3, 0, -3, 1), vec4(1, 1, 1, 1) };
	Vertex e4 = { vec4(0, 3, 0, 1), vec4(1, 1, 1, 1) };
	Vertex f5 = { vec4(3, 3, 0, 1), vec4(1, 1, 1, 1) };
	Vertex g6 = { vec4(0, 3, -3, 1), vec4(1, 1, 1, 1) };
	Vertex h7 = { vec4(3, 3, -3, 1), vec4(1, 1, 1, 1) };

	vector<Vertex> vertices{ a0, b1, c2, d3, e4, f5, g6, h7 };
	vector<unsigned int> indices{
		0,1,2,//bottom
		2,3,1,

		4,5,6,//top
		6,7,5,

		1,3,5,//right
		5,7,3,

		0,2,4,//left
		4,6,2,

		0,1,4,//front
		4,5,1,

		2,3,6,//back
		6,7,3
	};

	mesh->initialize(vertices, indices);
	mesh->Create_Buffers();

	vertices.clear();
	indices.clear();
	return false;
}

bool RenderingApp::shutdown()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) || m_GameOver)
		glfwSetWindowShouldClose(m_window, true);
	return false;
}

bool RenderingApp::update(float deltaTime)
{
	runTime += deltaTime;
	camapp->Keyboard_Movement(cam, m_window);
	camapp->Mouse_Movement(cam, m_window);
	return false;
}

bool RenderingApp::draw()
{
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	unsigned int time = glGetUniformLocation(m_programID, "time");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glUseProgram(m_programID);

	mesh->bind();
		rotationView = rotationView * rotate(0.1f, vec3(0, 1, 0));
		glUniform1f(time, glfwGetTime());
		glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView() * rotationView));
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, nullptr);
	mesh->unbind();

	mesh->bind();
		mat4 newModel1 = translate(vec3(5, 0, 0));
		glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView() * newModel1 * rotationView));
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, nullptr);
	mesh->unbind();

	mesh->bind();
		mat4 newModel2 = translate(vec3(5, 0, 5));
		glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView() * newModel2 * rotationView));
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, nullptr);
	mesh->unbind();


	mesh->bind();
		mat4 newModel3 = translate(vec3(0, 0, 5));
		glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView() * newModel3 * rotationView));
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, nullptr);
	mesh->unbind();

	glUseProgram(0);
	return false;
}