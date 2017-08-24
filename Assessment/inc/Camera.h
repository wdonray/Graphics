#pragma once
#include <glm/glm.hpp>
#include "Transform.h"
using namespace glm;
class Camera
{
public:
	Camera();
	virtual ~Camera();

	mat4 m_position;
	mat4 m_view;
	virtual void update(float deltaTime);
	void setPerspective(float FOV, float aspectRatio, float near, float far);
	void setOrthographicView(float top, float bot, float left, float right, float far, float near);
	void setLookAt(vec3 eye, vec3 center, vec3 up);
	void setPosition(vec3 position);
	void setSpeed(float speed);
	mat4 getWorldTransform() const;
	mat4 getView() const;
	mat4 getProjection() const;
	mat4 getProjectionView() const;
	Transform * m_transform;

private:
	float m_fov;
	float m_aspectRatio;
	float m_near;
	float m_far;
	float m_speed;

	mat4 m_worldTransform;
	mat4 m_projection;
	mat4 m_viewTransform;
	mat4 m_projectionTransform;
	mat4 m_projectionViewTransform;


	void updateProjectionViewTransfrom();
};

