#include "Camera.h"
Camera::Camera()
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
}

void Camera::setLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
}

void Camera::setPosition(glm::vec3 position)
{
}

glm::mat4 Camera::getWorldTransform()
{
	
}

glm::mat4 Camera::getView()
{

}

glm::mat4 Camera::getProjection()
{
}

glm::mat4 Camera::getProjectionView()
{
}

void Camera::updateProjectionViewTransfrom()
{
}
