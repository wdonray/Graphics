#include "CameraApp.h"
#include "Gizmos.h"
#include <GLFW/glfw3.h>
#include "Transform.h"
#include <iostream>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.h"


CameraApp::CameraApp() : m_camera(nullptr), runTime(0)
{
}

CameraApp::~CameraApp()
{
}

Transform* sun_transform;
Transform* earth_transform;
Transform* mercury_transform;
Transform* venus_transform;
Transform* mars_transform;

bool CameraApp::startup()
{
	sun_transform = new Transform();
	earth_transform = new Transform();
	mercury_transform = new Transform();
	venus_transform = new Transform();
	mars_transform = new Transform();
	glEnable(GL_DEPTH_TEST);
	//Sets color on startup
	setBackgroundColor(0.2f, 0.3f, 0.3f, 1.0f);
	Gizmos::create();
	m_camera = new Camera();
	m_camera->setLookAt(vec3(10, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0));
	ImGui_ImplGlfwGL3_Init(m_window, true);
	auto& io = ImGui::GetIO();
	io.DisplaySize.x = 1600;
	io.DisplaySize.y = 800;
	return false;
}

bool CameraApp::shutdown()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) || m_GameOver)
	{
		glfwSetWindowShouldClose(m_window, true);
		ImGui::Shutdown();
	}
	return false;
}
auto fps_window = true;
auto quit_window = true;
auto controls_window = true;
bool CameraApp::update(float deltaTime)
{
	runTime += deltaTime;

	//Setting the planets matrix
	Planets_Respect();
	//GetMousePos
	Mouse_Movement();
	//Move Camera with Keys
	Keyboard_Movement();
	//ImGui Text
	ImGuiWindows();

	m_camera->update(deltaTime);
	
	return false;
}

bool CameraApp::draw()
{
	auto s1 = mat4(1);
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	vec4 yellow(255, 255, 0, 1);
	vec4 gray(0.5f, .5f, .5f, 1);
	vec4 peru(0.8f, 0.5f, 0.2f, 1);
	vec4 blue(0, 191, 255, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	Gizmos::clear();
	mercury = sun * mercury * .2f;
	venus = sun * venus * .2f;
	earth = sun * earth * .2f;
	Gizmos::addSphere(mercury[3], 1, 20, 20, gray, &mercury);
	Gizmos::addSphere(venus[3], 2, 20, 20, peru, &venus);
	Gizmos::addSphere(earth[3], 2, 20, 20, blue, &earth);
	Gizmos::addSphere(sun_transform->m_world[3], 1, 20, 20, yellow, &sun_transform->m_world);
	Gizmos::addTransform(mercury, 3);
	Gizmos::addTransform(venus, 5);
	Gizmos::addTransform(earth, 5);
	Gizmos::addTransform(s1, 4);

	for (auto i = 0; i < 21; ++i)
	{
		Gizmos::addLine(
			vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? black : white);
		Gizmos::addLine(
			vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? black : white);
	}
	auto projview = m_camera->getProjectionView();
	Gizmos::draw(projview);
	ImGui::Render();
	return false;
}

void CameraApp::Keyboard_Movement() const
{
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
	if (glfwGetKey(m_window, 'T') == GLFW_PRESS)
	{
		m_camera->setLookAt(vec3(10, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0));
	}
}

void CameraApp::Mouse_Movement()
{
	static auto MouseButtonDown = false;
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		//If we pressed then set the starting points to zero
		static double PrevMouseX = 0, PrevMouseY = 0;
		//If we let go of the mouse button
		if (MouseButtonDown == false)
		{
			//Set the mouse down to true and save the starting mouse pos
			MouseButtonDown = true;
			glfwGetCursorPos(m_window, &PrevMouseX, &PrevMouseY);
		}
		//Always calculate the current mouse pos
		double currmouseX = 0, currmouseY = 0;
		glfwGetCursorPos(m_window, &currmouseX, &currmouseY);
		//Calculate the delta pos if we did not press the mouse this will be 0 - 0 = 0 
		auto DeltaX = currmouseX - PrevMouseX;
		auto DeltaY = currmouseY - PrevMouseY;
		//Set the previous mouse to the current mouse
		PrevMouseX = currmouseX;
		PrevMouseY = currmouseY;
		auto rEle = rotate(static_cast<float>(DeltaX) * 1 / 800, vec3(0, 1, 0));
		auto rAzi = rotate(static_cast<float>(DeltaY) * 1 / 800, vec3(1, 0, 0));
		m_camera->m_view = rEle * rAzi * m_camera->m_view;

		std::cout << "Delta Mouse:: " << to_string(vec2(DeltaX, DeltaY)) << std::endl;
	}
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_3))
	{
		printf("worked");
	}
	mercury = sun_transform->m_world * translate(vec3(0, 0, 10)) * rotate(runTime, vec3(0, 1, 0));
	venus = sun_transform->m_world * translate(vec3(0, 0, 20)) * rotate(runTime * .5f, vec3(0, 1, 0));
	earth = sun_transform->m_world * translate(vec3(0, 0, 30)) * rotate(runTime * .25f, vec3(0, 1, 0));
	sun_transform->rotate(runTime, YAXIS);
	//earth_transform->rotate(runTime, XAXIS);
}

void CameraApp::Planets_Respect()
{
	mercury = mat4(
		cos(runTime), 0, sin(runTime), 0,
		0, 1, 0, 0,
		-sin(runTime), 0, cos(runTime), 0,
		10, 0, 0, 1
	);
	venus = mat4(
		cos(runTime), 0, sin(runTime), 0,
		0, 1, 0, 0,
		-sin(runTime), 0, cos(runTime), 0,
		10, 0, 0, 1
	);
	earth = mat4(
		cos(runTime), 0, sin(runTime), 0,
		0, 1, 0, 0,
		-sin(runTime), 0, cos(runTime), 0,
		10, 0, 0, 1
	);
	mars = mat4(
		cos(runTime), 0, sin(runTime), 0,
		0, 1, 0, 0,
		-sin(runTime), 0, cos(runTime), 0,
		10, 0, 0, 1
	);
}

void CameraApp::ImGuiWindows()
{
	ImGui_ImplGlfwGL3_NewFrame();

	ImGui::Begin("FPS", &fps_window);
	ImGui::SetWindowPos(ImVec2(0, 20));
	ImGui::Text("Application FPS (%.1f FPS)", ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Begin("Quit", &fps_window);
	ImGui::SetWindowPos(ImVec2(1450, 20));
	ImGui::Checkbox("<-Select to Exit", &m_GameOver);
	ImGui::End();

	ImGui::Begin("Controls", &controls_window);
	ImGui::SetWindowPos(ImVec2(0, 500));
	const auto white = ImVec4(1, 1, 1, 1);
	const auto red = ImVec4(1, 0, 0, 1);
	ImGui::TextColored(white, "W:");
	ImGui::TextColored(red, " Moves Camera Forward");

	ImGui::TextColored(white, "A:");
	ImGui::TextColored(red, " Moves Camera Left");

	ImGui::TextColored(white, "S:");
	ImGui::TextColored(red, " Moves Camera Back");

	ImGui::TextColored(white, "D:");
	ImGui::TextColored(red, " Moves Camera Right");

	ImGui::TextColored(white, "Q:");
	ImGui::TextColored(red, " Moves Camera Up");

	ImGui::TextColored(white, "E:");
	ImGui::TextColored(red, " Moves Camera Down");

	ImGui::TextColored(white, "R:");
	ImGui::TextColored(red, " Set Eye to vec3(10, 10, 10)");

	ImGui::TextColored(white, "T:");
	ImGui::TextColored(red, " Set Eye to vec3(10, 0, 0)");

	ImGui::End();
}

void CameraApp::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}
