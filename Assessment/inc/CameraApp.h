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
	void Mouse_Movement() const;
	void Planets_Respect();
	void Planet_Orbat();
	void ImGuiWindows();
protected:
	Camera * m_camera;
	float runTime;
	mat4 sun;
	mat4 mercury;
	mat4 venus;
	mat4 earth;
	mat4 mars;
	mat4 jupiter;
	mat4 saturn;
	mat4 uranus;
	mat4 neptune;
};

