#include "Camera.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/constants.inl>

Camera::Camera() : m_fov(0), m_aspectRatio(0), m_near(0), m_far(0), m_worldTransform(1), m_projection(1), m_viewTransform(1), m_projectionTransform(1), m_projectionViewTransform(1)
{
	setPerspective(pi<float>()/ 4.f, 16 / 9.f, 0.1f, 1000.f);
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
	auto y = 1.f / (tan(m_fov / 2.f));
	auto z = -1.f * ((m_far + m_near) / (m_far - m_near));
	auto w = -1.f * ((2.f * m_far * m_near) / (m_far - m_near));

	m_projection = mat4(
		vec4(x, 0, 0, 0),
		vec4(0, y, 0, 0),
		vec4(0, 0, z, -1),
		vec4(0, 0, w, 0));
	auto copy = perspective(m_fov, aspectRatio, near, far);
	//m_projection = copy;
	assert(m_projection == copy);
}
// ReSharper disable CppUseAuto
void Camera::setLookAt(vec3 eye, vec3 center, vec3 up)
{
	//vec3 f = eye - center;
	//f = normalize(f);
	//vec3 z = f;
	//vec3 s = cross(up, z);
	//vec3 x = normalize(s);
	//vec3 v = cross(z, x);
	//vec3 y = normalize(v);
	vec3 z = normalize(eye - center);
	vec3 x = normalize(cross(up, z));
	vec3 y = cross(z, x);
	mat4 view = mat4(
		x[0], y[0], z[0], 0,
		x[1], y[1], z[1], 0,
		x[2], y[2], z[2], 0,
		0, 0, 0, 1);
	mat4 translate = mat4(
		vec4(1, 0, 0, 0),
		vec4(0, 1, 0, 0),
		vec4(0, 0, 1, 0),
		vec4(-eye.x, -eye.y, -eye.z, 1));
	view = view * translate;
	m_view = view;
	mat4 expected = lookAt(eye, center, up);
	assert(view == expected);
	m_worldTransform = inverse(view);
	m_projectionViewTransform = m_projection * view;
}

void Camera::setPosition(vec3 position)
{
	m_viewTransform = translate(m_worldTransform, position);
}

mat4 Camera::getWorldTransform() const
{
	return m_worldTransform;
}

mat4 Camera::getView() const
{
	return m_viewTransform;
}

mat4 Camera::getProjection() const
{
	return m_projectionTransform;
}

mat4 Camera::getProjectionView() const
{
	return m_projection * m_view;
}

void Camera::updateProjectionViewTransfrom()
{
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}
