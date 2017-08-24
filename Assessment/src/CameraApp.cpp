#include "CameraApp.h"
#include "Gizmos.h"
#include <GLFW/glfw3.h>
#include "Transform.h"


CameraApp::CameraApp() : m_camera(nullptr), rt(0)
{
}

CameraApp::~CameraApp()
{
}
Transform * sun_transform;
Transform * earth_transform;
mat3 lookAtOrigin = mat3(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
bool CameraApp::startup()
{
	sun_transform = new Transform();
	earth_transform = new Transform();
	glEnable(GL_DEPTH_TEST);
	//Sets color on startup
	setBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);
	Gizmos::create();
	m_camera = new Camera();
	m_camera->setLookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
	return false;
}

bool CameraApp::shutdown()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_window, true);
	return false;
}

bool CameraApp::update(float deltaTime)
{
	rt += deltaTime;
	earth_wrt_sun = mat4(
		cos(rt), 0, -sin(rt), 0,
		0, 1, 0, 0,
		sin(rt), 0, cos(rt), 0,
		10, 0, 0, 1
	);
	sun_transform->rotate(rt, YAXIS);
	if (glfwGetKey(m_window, 'A') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] -= m_camera->getWorldTransform()[0]);
	}
	if (glfwGetKey(m_window, 'W') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] -= m_camera->getWorldTransform()[2]);
	}
	if (glfwGetKey(m_window, 'S') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] += m_camera->getWorldTransform()[2]);
	}
	if (glfwGetKey(m_window, 'D') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] += m_camera->getWorldTransform()[0]);
	}
	if (glfwGetKey(m_window, 'Q') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] += m_camera->getWorldTransform()[1]);
	}
	if (glfwGetKey(m_window, 'E') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] -= m_camera->getWorldTransform()[1]);
	}
	if (glfwGetKey(m_window, 'R') == GLFW_PRESS)
	{
		m_camera->setLookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
	}
	m_camera->update(deltaTime);
	return false;
}

bool CameraApp::draw()
{
	mat4 s1 = mat4(1);
	vec4 center = vec4(0, 0, 0, 1);
	vec4 color = vec4(0, 0, 0, 0);
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	Gizmos::clear();
	earth_wrt_sun = sun_wrt_world * earth_wrt_sun * 2.f;
	Gizmos::addSphere(earth_wrt_sun[3], 1, 20, 20, color, &earth_wrt_sun);
	mat4 sun4x4 = sun_transform->m_world;
	Gizmos::addSphere(sun_transform->m_world[3], 1, 20, 20, color, &sun4x4);
	Gizmos::addTransform(earth_wrt_sun, 4);
	//Gizmos::addSphere(s1[3], 1, 20, 20, color);
	Gizmos::addTransform(s1, 4);

	for (auto i = 0; i < 21; ++i)
	{
		Gizmos::addLine(
			vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(
			vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}
	auto projview = m_camera->getProjectionView();
	Gizmos::draw(projview);
	return false;
}
