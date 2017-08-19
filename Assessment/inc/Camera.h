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
	void setLookAt(vec3 from, vec3 to, vec3 up);
	void setPosition(vec3 position);
	mat4 getWorldTransform();
	mat4 getView();
	mat4 getProjection();
	mat4 getProjectionView();


private:
	float m_fov;
	float m_aspectRatio;
	float m_near;
	float m_far;

	mat4 worldTransform;
	mat4 m_projection;
	mat4 viewTransform;
	mat4 projectionTransform;
	mat4 projectionViewTransform;

	void updateProjectionViewTransfrom();
};

