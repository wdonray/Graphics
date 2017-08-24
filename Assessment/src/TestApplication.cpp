#include "TestApplication.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <Gizmos.h>

TestApplication::TestApplication() /*: shader_programme(0)*/
{
}

TestApplication::~TestApplication()
{
}

bool TestApplication::startup()
{
	//Idk yet
	glEnable(GL_DEPTH_TEST);
	//Sets color on startup
	setBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);

	//test
	//From Hello World
	//float points[] = {
	//	0.0f,  0.5f,  0.0f,
	//	0.5f, -0.5f,  0.0f,
	//	-0.5f, -0.5f,  0.0f
	//};
	//GLuint vbo = 0;
	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
	//vao = 0;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	//auto vertex_shader =
	//	"#version 400\n"
	//	"in vec3 vp;"
	//	"void main() {"
	//	"  gl_Position = vec4(vp, 1.0);"
	//	"}";
	//auto fragment_shader =
	//	"#version 400\n"
	//	"out vec4 frag_colour;"
	//	"void main() {"
	//	"  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
	//	"}";
	//auto vs = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vs, 1, &vertex_shader, nullptr);
	//glCompileShader(vs);
	//auto fs = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fs, 1, &fragment_shader, nullptr);
	//glCompileShader(fs);
	//shader_programme = glCreateProgram();
	//glAttachShader(shader_programme, fs);
	//glAttachShader(shader_programme, vs);
	//glLinkProgram(shader_programme);
	return false;
}

bool TestApplication::update(float deltaTime)
{
	// NO REASON FOR THIS :: Whenever you press Space the background color will change
	auto r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	auto r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (glfwGetKey(m_window, GLFW_KEY_SPACE))
		setBackgroundColor(r, r2, 0.3f, 1.0f);
	return false;
}

bool TestApplication::draw()
{
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	//glUseProgram(shader_programme);
	//glBindVertexArray(vao);
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	return false;
}
bool TestApplication::shutdown()
{
	//Shutdowns application if the escape key is pressed.
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_window, true);
	return false;
}