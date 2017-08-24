#include "TestApplication.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <Gizmos.h>

TestApplication::TestApplication() /*: shader_programme(0)*/ : m_camera(nullptr)
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
	Gizmos::create();
	m_camera = new Camera();
	m_camera->setLookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
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
mat4 s1 = mat4(1);
vec4 center = vec4(0, 0, 0, 1);
vec4 color = vec4(0, 0, 0, 0);
bool TestApplication::draw()
{
	//Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	Gizmos::clear();
	Gizmos::addSphere(s1[3], 1, 20, 20, color);
	Gizmos::addTransform(s1, 4);
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (auto i = 0; i < 21; ++i)
	{
		Gizmos::addLine(
			glm::vec3(-10 + i, 0, 10),
			glm::vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(
			glm::vec3(10, 0, -10 + i),
			glm::vec3(-10, 0, -10 + i),
			i == 10 ? white : black);

	}
	auto projview = m_camera->getProjectionView();
	Gizmos::draw(projview);
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