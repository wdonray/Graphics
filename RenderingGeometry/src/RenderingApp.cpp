#include "RenderingApp.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>


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
	setBackgroundColor(0.4f, 0.0f, 0.8f, 1.0f);
	cam->setLookAt(vec3(20, 20, 20), vec3(5, 0, 5), vec3(0, 1, 0));
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

	Vertex a = { vec4(-5, 0, 0, 1), vec4(1, 1, 1, 1) };//bl	
	Vertex b = { vec4(5, 0, 0, 1), vec4(1, 1, 1, 1) };//br
	Vertex c = { vec4(5, -5, 0, 1), vec4(1, 1, 1, 1) };//tl
	Vertex d = { vec4(-5, -5, 0, 1), vec4(1, 1, 1, 1) };//tr
	Vertex e = { vec4(-5, 5, 0, 1), vec4(1, 1, 1, 1) };//tr	
	Vertex f = { vec4(5, 5, 0, 1) , vec4(1, 1, 1, 1) };

	vector<Vertex> vertices{ a,b,c,d,e,f };
	vector<unsigned int> indices{
		0, 1, 2,
		0, 2, 3,
		0, 4, 1,
		0, 4 ,5,
		1, 0 ,4 };

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
	//
	mesh->bind();

	glUniform1f(time, glfwGetTime());
	glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView()));
	glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, nullptr);

	mesh->unbind();
	//
	glUseProgram(0);
	return false;
}
