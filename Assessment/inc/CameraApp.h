#pragma once
#include "Application.h"
class CameraApp :
	public Application
{
public:
	CameraApp();
	virtual ~CameraApp();

	bool startup() override;
	bool shutdown() override;
	bool update(float deltaTime) override;
	bool draw() override;
	void Keyboard_Movement() const;
	void Mouse_Movement();
	void Planets_Respect();
	void ImGuiWindows();
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	Camera * m_camera;
	float runTime;
	mat4 mercury;
	mat4 venus;
	mat4 earth;
	mat4 mars;
	mat4 sun;
};

