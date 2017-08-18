#include "Application.h"
#include <glm/glm.hpp>
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>


Application::Application() : m_window(nullptr), m_GameOver(false)
{
}

Application::~Application()
{
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void Application::setBackgroundColor(float r, float g, float b, float a) 
{
	glClearColor(r, g, b, a);
}
// ReSharper disable once CppMemberFunctionMayBeStatic
float Application::getTime() const
{
	return float(glfwGetTime());
}
bool Application::createWindow(const char* title, unsigned int width, unsigned int height, bool fullscreen)
{
	if (!glfwInit())
		return false;
	auto monitor = (fullscreen) ? glfwGetPrimaryMonitor() : nullptr;
	m_window = glfwCreateWindow(width, height, title, monitor, nullptr);
	glfwMakeContextCurrent(m_window);
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}
	assert(m_window != nullptr);
	return true;
}

bool Application::run(const char* title, unsigned int width, unsigned int height, bool fullscreen)
{
	if (title == nullptr || width == 0 || height == 0)
		return false;
	createWindow(title, width, height, fullscreen);
	startup();
	float deltaTime = 0;
	float currentTime = glfwGetTime();
	float previousTime = 0;

	while (!m_GameOver)
	{
		currentTime = getTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		glfwPollEvents();
		update(deltaTime);
		draw();
		shutdown();
		glfwSwapBuffers(m_window);
		m_GameOver = m_GameOver || glfwWindowShouldClose(m_window) == true;
	}
	return false;
}
