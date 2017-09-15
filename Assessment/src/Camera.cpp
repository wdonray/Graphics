#include "Camera.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/constants.inl>

Camera::Camera() : m_posvec3(5,5,5), m_fov(0), m_aspectRatio(0), m_near(0),
                   m_far(0), m_worldTransform(1), m_projection(1), m_projectionViewTransform(1)
{
	setPerspective(pi<float>() / 4.f, 16 / 9.f, 0.1f, 1000.f);
	//setOrthographicView(-15, 15, -15, 20, -10, 50);
	m_transform = new Transform();
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

// ReSharper disable once CppMemberFunctionMayBeConst
// ReSharper disable once CppMemberFunctionMayBeStatic
void Camera::setOrthographicView(float left, float right, float bot, float top, float near, float far)
{
	mat4 expected = ortho(left, right, bot, top, near, far);
	float x = 2.f / (right - left);
	float y = 2.f / (top - bot);
	float z = -2.f / (far - near);

	float xt = -1 * ((right + left) / (right - left));
	float yt = -1 * ((top + bot) / (top - bot));
	float zt = -1 * ((far + near) / (far - near));

	// ReSharper disable once CppUseAuto
	mat4 orthoView = mat4(
		vec4(x, 0, 0, 0),
		vec4(0, y, 0, 0),
		vec4(0, 0, z, 0),
		vec4(xt, yt, zt, 1)
	);

	assert(orthoView == expected);
	m_projection = orthoView;
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
	m_worldTransform[3] = vec4(position, 1);
	m_transform->rotate(pi<float>() * 0.25f, ZAXIS);
	m_transform->translate(position);
	m_view = inverse(m_worldTransform);

	this->m_posvec3 = this->m_posvec3 + position;
}

void Camera::setSpeed(float speed)
{
	m_speed = speed;
}

mat4 Camera::getWorldTransform() const
{
	return m_worldTransform;
}

mat4 Camera::getView() const
{
	return m_view;
}

mat4 Camera::getProjection() const
{
	return m_projection;
}

mat4 Camera::getProjectionView() const
{
	return m_projection * m_view;
}
