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
	/**
	 * Keyboard movement using the keyboard
	 */
	void Keyboard_Movement() const;
	/**
	 * Mouse movement using the mouse 
	 */
	void Mouse_Movement() const;
	/**
	 * Planets with respect to the sun
	 */
	void Planets_Respect();
	/**
	 * Orbits the sun 
	 */
	void Planet_Orbit();
	/**
	 * Used to access and use the ImGui Library
	*/
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

