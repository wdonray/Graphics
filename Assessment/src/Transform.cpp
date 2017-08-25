#include "Transform.h"
#include <glm/ext.hpp>


Transform::Transform() : m_world(1), m_local(1), m_rotation(1), m_pos(0), m_scale(1)
{
}


Transform::~Transform()
{
}

void Transform::rotate(float radians, Axis rotationAxis)
{
	vec3 xaxis = vec3(1, 0, 0);
	vec3 yaxis = vec3(0, 1, 0);
	vec3 zaxis = vec3(0, 0, 1);
	//m_rotation = mat4(1);
	vec3 rotaxis = (rotationAxis == ZAXIS) ? vec3(0, 0, 1) : (rotationAxis == YAXIS) ? vec3(0, 1, 0) : vec3(1, 0, 0);
	switch (rotationAxis)
	{
	case ZAXIS:
		xaxis = vec3(cos(radians), sin(radians), 0);
		yaxis = vec3(-sin(radians), cos(radians), 0);
		zaxis = vec3(0, 0, 1);
		break;
	case YAXIS:
		xaxis = vec3(cos(radians), 0, -sin(radians));
		yaxis = vec3(0, 1, 0);
		zaxis = vec3(sin(radians), 0, cos(radians));
		break;
	case XAXIS:
		xaxis = vec3(1, 0, 0);
		yaxis = vec3(0, cos(radians), sin(radians));
		zaxis = vec3(0, -sin(radians), cos(radians));
		break;
	default:
		break;
	}
	m_rotation = mat4(vec4(xaxis, 1), vec4(yaxis, 1), vec4(zaxis, 1), vec4(0, 0, 0, 1));
	m_world = mat4(1) * m_rotation *m_scale;
	//mat4 expexted = glm::rotate(radians, rotaxis);
	//assert(m_rotation == expexted);
}

void Transform::translate(vec3 trans)
{
	mat4 translation = mat4(1);
	translation[3].x = trans.x;
	translation[3].y = trans.y;
	translation[3].z = trans.z;
	m_world = m_world * translation;

	mat4 expexted = glm::translate(m_world, trans);
}
