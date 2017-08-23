#pragma once
#include <glm/detail/type_vec3.hpp>
#include "Camera.h"

class FlyCamera : Camera
{
public:
	FlyCamera();
	~FlyCamera();
	void update(float deltaTime)override;
	void setSpeed(float speed);
private:
	float speed;
	vec3 up;
};

