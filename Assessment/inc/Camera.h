#pragma once
#include <glm/glm.hpp>
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
	void setLookAt(vec3 eye, vec3 center, vec3 up);
	void setPosition(vec3 position);
	mat4 getWorldTransform() const;
	mat4 getView() const;
	mat4 getProjection() const;
	mat4 getProjectionView() const;


private:
	float m_fov;
	float m_aspectRatio;
	float m_near;
	float m_far;

	mat4 m_worldTransform;
	mat4 m_projection;
	mat4 m_viewTransform;
	mat4 m_projectionTransform;
	mat4 m_projectionViewTransform;

	void updateProjectionViewTransfrom();
};

