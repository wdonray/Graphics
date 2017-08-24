#pragma once
#include <glm/glm.hpp>
using namespace glm;

enum Axis
{
	XAXIS,
	YAXIS,
	ZAXIS
};

class Transform
{
public:
	Transform();
	~Transform();
	mat4 m_world;
	mat4 m_local;
	mat4 m_rotation;
	vec3 m_pos;
	float m_scale;

	void rotate(float radians, Axis rotationAxis);
	void translate(vec3 trans);
};
