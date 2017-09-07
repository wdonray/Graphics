#include "RenderingApp.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include "Camera.h"
#include "LoadFile.h"
#include "CameraApp.h"
#include "Mesh.h"
#include "Shader.h"
#define PI 3.14159265359


RenderingApp::RenderingApp() : m_VAO(0), m_VBO(0), m_IBO(0), m_programID(0),
cam(nullptr), fl(nullptr), shader(nullptr), vsSource(nullptr),
fsSource(nullptr), m_rows(0), m_cols(0), runTime(0), slice(0), theta(0)
{
	cam = new Camera();
	fl = new LoadFile();
	camapp = new CameraApp();
	cubeMesh = new Mesh();
	sphereMesh = new Mesh();
	shader = new Shader();
	rotationView = mat4
	(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-5, 0, -5, 1
	);
}

RenderingApp::~RenderingApp()
{
	delete cam;
	delete fl;
	delete camapp;
	delete cubeMesh;
	delete shader;
	delete sphereMesh;
}

Mesh* RenderingApp::generateHalfCircle(float radius, float numPoints)
{
	auto sphereVerts = vector<Vertex>();
	float x = radius;
	float y = 0;
	float z = 0;
	for (auto i = 0; i < numPoints; i++)
	{
		slice = PI / (numPoints - 1);
		theta = i * slice;
		x = radius * sin(theta);
		y = radius * cos(theta);
		z = 0;

		auto test = Vertex();
		test.position = vec4(x, y, z, 1);
		test.color = vec4(x, y, z, 1);
		sphereVerts.push_back(test);
	}
	sphereMesh->initialize(sphereVerts, vector<unsigned int>());
	return sphereMesh;
}

Mesh* RenderingApp::generateCube()
{

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

	cubeMesh->initialize(vertices, indices);
	return cubeMesh;
}

Mesh* RenderingApp::rotatePoints(vector<vec4> points, float numMeridians)
{
	float phi = two_pi<float>() / slice;
	float newX, newY, newZ;
	for (int i = 0; i < points.size(); i++)
	{
		for (int j = 0; j < numMeridians; j++)
		{
			newX = points[j].x * cos(phi) + points[j].z * sin(phi);
			newY = points[j].y;
			newZ = points[j].z * cos(phi) - points[j].x * sin(phi);
		}
	}
	return sphereMesh;
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

	shader->load("vsSource.vert", GL_VERTEX_SHADER);
	shader->load("fsSource.vert", GL_FRAGMENT_SHADER);
	shader->attach();

	cubeMesh = generateCube();
	cubeMesh->Create_Buffers();

	sphereMesh = generateHalfCircle(1, 1000);
	sphereMesh = rotatePoints()
		sphereMesh->Create_Buffers();
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
	//unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	//unsigned int time = glGetUniformLocation(m_programID, "time");
	unsigned int projectionViewUniform = shader->getUniform("projectionViewWorldMatrix");
	unsigned int time = shader->getUniform("time");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	shader->bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*cubeMesh->bind();
	rotationView = rotationView * rotate(0.1f, vec3(0, 1, 0));
	glUniform1f(time, glfwGetTime());
	glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView() * rotationView));
	glDrawElements(GL_TRIANGLES, cubeMesh->index_count, GL_UNSIGNED_INT, nullptr);
	cubeMesh->unbind();

	cubeMesh->bind();
	mat4 newModel1 = translate(vec3(5, 0, 0));
	glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView() * newModel1 * rotationView));
	glDrawElements(GL_TRIANGLES, cubeMesh->index_count, GL_UNSIGNED_INT, nullptr);
	cubeMesh->unbind();

	cubeMesh->bind();
	mat4 newModel2 = translate(vec3(5, 0, 5));
	glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView() * newModel2 * rotationView));
	glDrawElements(GL_TRIANGLES, cubeMesh->index_count, GL_UNSIGNED_INT, nullptr);
	cubeMesh->unbind();

	cubeMesh->bind();
	mat4 newModel3 = translate(vec3(0, 0, 5));
	glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView() * newModel3 * rotationView));
	glDrawElements(GL_TRIANGLES, cubeMesh->index_count, GL_UNSIGNED_INT, nullptr);
	cubeMesh->unbind();
*/
	rotationView = rotationView * rotate(0.1f, vec3(0, 1, 0));
	sphereMesh->bind();
	glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView() * translate(vec3(0, -.5, 0))
		* rotationView));
	glDrawArrays(GL_POINTS, 0, sphereMesh->vertRef.size());
	sphereMesh->unbind();

	//How to Scale
	cubeMesh->bind();
	mat4 newModel4 = glm::scale(glm::mat4(1), glm::vec3(.1f, .1f, .1f));
	glUniformMatrix4fv(projectionViewUniform, 1, false, value_ptr(cam->getProjectionView() * newModel4 *
		rotationView * translate(vec3(0, 25, 0))));
	glDrawElements(GL_TRIANGLES, cubeMesh->index_count, GL_UNSIGNED_INT, nullptr);
	cubeMesh->unbind();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader->unbind();
	return false;
}
