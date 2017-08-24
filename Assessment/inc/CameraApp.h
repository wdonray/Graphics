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
	Camera * m_camera;
	float rt;
	mat4 earth_wrt_sun;
	mat4 sun_wrt_world;
};

