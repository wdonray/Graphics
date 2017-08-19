#include "Camera.h"
#include <glm/gtc/matrix_transform.inl>

Camera::Camera(): m_fov(0), m_aspectRatio(0), m_near(0), m_far(0)
{
}

Camera::~Camera()
{
}

void Camera::update(float deltaTime)
{
}

void Camera::setPerspective(float FOV, float aspectRatio, float near, float far)
{
	m_fov = FOV;
	m_aspectRatio = aspectRatio;
	m_near = near;
	m_far = far;
	auto x = 1.f / (m_aspectRatio * tan(m_fov / 2.f));
	auto y = 1.f / (m_aspectRatio * tan(m_fov / 2.f));
	auto z = -1.f * ((m_far + m_near) / (m_far - m_near));
	auto w = -1.f * ((2.f * m_far * m_near) / (m_far - m_near));

	m_projection = mat4(
		vec4(x, 0, 0, 0),
		vec4(0, y, 0, 0),
		vec4(1, 0, z, -1.f),
		vec4(0, 0, w, 0));
	auto copy = perspective(m_fov, 16 / 9.f, 0.1f, 1000.f);
}

void Camera::setLookAt(vec3 from, vec3 to, vec3 up)
{
}

void Camera::setPosition(vec3 position)
{
}

mat4 Camera::getWorldTransform()
{
	return {};
}

mat4 Camera::getView()
{
	return {};
}

mat4 Camera::getProjection()
{
	return {};
}

mat4 Camera::getProjectionView()
{
	return {};
}

void Camera::updateProjectionViewTransfrom()
{
}
