#include "LightingApp.h"
#include "CameraApp.h"
#include <gl_core_4_4.h>
#include "Camera.h"
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <glm/ext.hpp>
#include <Mesh.h>

#define PI 3.14159265359

LightingApp::LightingApp() : runTime(0), m_VAO(0), m_VBO(0), m_IBO(0), index_count(0), m_modelMatrix(1)
{
	cam = new Camera();
	camapp = new CameraApp();
	shader = new Shader();
}

LightingApp::~LightingApp()
{
	delete camapp;
	delete cam;
	delete shader;
}

void LightingApp::generateSphere(unsigned segments, unsigned rings, unsigned& vao, unsigned& vbo, unsigned& ibo, unsigned& indexCount)
{
	unsigned int vertCount = (segments + 1) * (rings + 2);
	indexCount = segments * (rings + 1) * 6;

	// using AIEVertex for now, but could be any struct as long as it has the correct elements
	Vertex* vertices = new Vertex[vertCount];
	unsigned int* indices = new unsigned int[indexCount];

	float ringAngle = pi<float>() / (rings + 1);
	float segmentAngle = 2.0f * pi<float>() / segments;

	Vertex* vertex = vertices;

	for (unsigned int ring = 0; ring < (rings + 2); ++ring)
	{
		float r0 = sin(ring * ringAngle);
		float y0 = cos(ring * ringAngle);

		for (unsigned int segment = 0; segment < (segments + 1); ++segment , ++vertex)
		{
			float x0 = r0 * sin(segment * segmentAngle);
			float z0 = r0 * cos(segment * segmentAngle);

			vertex->position = vec4(x0 * 0.5f, y0 * 0.5f, z0 * 0.5f, 1);
			vertex->normal = vec4(x0, y0, z0, 0);

			vertex->tangent = vec4(sin(segment * segmentAngle + half_pi<float>()), 0, cos(segment * segmentAngle + half_pi<float>()), 0);

			// not a part of the AIEVertex, but this is how w generate bitangents
			vertex->bitangent = vec4(cross(vec3(vertex->normal), vec3(vertex->tangent)), 0);

			vertex->texcoord = vec2(segment / (float)segments, ring / (float)(rings + 1));
		}
	}

	unsigned int index = 0;
	for (unsigned i = 0; i < (rings + 1); ++i)
	{
		for (unsigned j = 0; j < segments; ++j)
		{
			indices[index++] = i * (segments + 1) + j;
			indices[index++] = (i + 1) * (segments + 1) + j;
			indices[index++] = i * (segments + 1) + (j + 1);

			indices[index++] = (i + 1) * (segments + 1) + (j + 1);
			indices[index++] = i * (segments + 1) + (j + 1);
			indices[index++] = (i + 1) * (segments + 1) + j;
		}
	}

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &vao);

	// all changes will apply to this handle
	glBindVertexArray(vao);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

	// colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(vec4) * 2));

	// texcoords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4) * 3));

	// tangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(vec4) * 3 + sizeof(vec2)));

	// safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;
	delete[] vertices;
}

bool LightingApp::startup()
{
	setBackgroundColor(1, 1, 1, 1.0f);
	cam->setLookAt(cam->m_posvec3, vec3(0), vec3(0, 1, 0));

	m_directLight.diffuse = vec3(0, 1, 0);
	m_directLight.specular = vec3(1);
	m_ambientLight = vec3(0, 0.25f, 0);

	m_material.diffuse = vec3(1);
	m_material.ambient = vec3(1);
	m_material.specular = vec3(1);

	m_material.specularPower = 30;
	generateSphere(100, 100, m_VAO, m_VBO, m_IBO, index_count);

	shader->load("phong.vert", GL_VERTEX_SHADER);
	shader->load("phong.frag", GL_FRAGMENT_SHADER);
	shader->attach();
	m_modelMatrix = scale(vec3(5));
	return false;
}

bool LightingApp::shutdown()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) || m_GameOver)
		glfwSetWindowShouldClose(m_window, true);
	return false;
}

bool LightingApp::update(float deltaTime)
{
	runTime += deltaTime;
	camapp->Keyboard_Movement(cam, m_window);
	camapp->Mouse_Movement(cam, m_window);

	m_directLight.direction = normalize(vec3(sinf(runTime / 2.f), 0, cosf(runTime / 2.f)));

	if (glfwGetKey(m_window, GLFW_KEY_F1))
	{
		m_material.specularPower += .2f;
		printf("Raised Specular Power by .2: %f\n", m_material.specularPower);
	}
	if (glfwGetKey(m_window, GLFW_KEY_F2))
	{
		m_material.specularPower -= .2f;
		printf("Lower Specular Power by .2: %f\n", m_material.specularPower);
	}
	if (glfwGetKey(m_window, GLFW_KEY_F3))
		m_directLight.direction = normalize(vec3(sinf(runTime / 2.f), 0, cosf(runTime / 2.f)));
	if (glfwGetKey(m_window, GLFW_KEY_F4))
		m_directLight.direction = normalize(vec3(10, 10, 10));

	return false;
}

bool LightingApp::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	shader->bind();
	mat4 pvm = cam->getProjectionView() * m_modelMatrix;

	int matUniform = shader->getUniform("ProjectionViewModel");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &pvm[0][0]);

	int lightUniform = shader->getUniform("direction");
	glUniform3fv(lightUniform, 1, value_ptr(m_directLight.direction));

	lightUniform = shader->getUniform("Id");
	glUniform3fv(lightUniform, 1, value_ptr(m_directLight.diffuse));

	lightUniform = shader->getUniform("Ia");
	glUniform3fv(lightUniform, 1, value_ptr(m_ambientLight));

	lightUniform = shader->getUniform("Is");
	glUniform3fv(lightUniform, 1, value_ptr(m_directLight.specular));

	lightUniform = shader->getUniform("Ka");
	glUniform3fv(lightUniform, 1, value_ptr(m_material.ambient));

	lightUniform = shader->getUniform("Kd");
	glUniform3fv(lightUniform, 1, value_ptr(m_material.diffuse));

	lightUniform = shader->getUniform("Ks");
	glUniform3fv(lightUniform, 1, value_ptr(m_material.specular));

	lightUniform = shader->getUniform("a");
	glUniform1f(lightUniform, m_material.specularPower);

	lightUniform = shader->getUniform("camPos");
	//glUniform3fv(lightUniform, 1, value_ptr(glm::vec3(cam->getProjectionView()[3][0], cam->getProjectionView()[3][1], cam->getProjectionView()[3][2])));
	glUniform3fv(lightUniform, 1, value_ptr(cam->m_posvec3));

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
	shader->unbind();
	return false;
}
