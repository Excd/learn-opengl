#include "camera.hpp"

namespace {
	// Camera defaults.
	const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);
	const float DEFAULT_YAW = -90.0f;
	const float DEFAULT_PITCH = 0.0f;
	const float DEFAULT_SPEED = 2.5f;
	const float DEFAULT_SENSITIVITY = 0.1f;
	const float DEFAULT_FOV_Y = 45.0f;
}

Camera::Camera() :
	position(ORIGIN),
	xAxis(RIGHT),
	yAxis(UP),
	zAxis(FORWARD),
	yaw(DEFAULT_YAW),
	pitch(DEFAULT_PITCH),
	speed(DEFAULT_SPEED),
	sensitivity(DEFAULT_SENSITIVITY),
	fovY(DEFAULT_FOV_Y) {}

Camera::Camera(glm::vec3 position) : Camera() {
	this->position = position;
	update();
}

Camera::Camera(glm::vec3 position, float yaw, float pitch) : Camera(position) {
	this->yaw = yaw;
	this->pitch = pitch;
	update();
}

Camera::~Camera() {}

void Camera::translate(glm::vec3 translation, float deltaTime) {
	position += translation * speed * deltaTime;
	speed = DEFAULT_SPEED; // Reset any speed modifiers.
}

void Camera::rotate(float xOffset, float yOffset, bool constrainPitch) {
	const float minPitch = -89.0f, maxPitch = 89.0f;

	yaw += xOffset * sensitivity;
	pitch += yOffset * sensitivity;

	if (constrainPitch)
		pitch = glm::clamp(pitch, minPitch, maxPitch);

	update();
}

void Camera::zoom(float yOffset) {
	const float minFovY = 1.0f, maxFovY = DEFAULT_FOV_Y;

	fovY = glm::clamp(fovY - yOffset, minFovY, maxFovY);
}

void Camera::accelerate(float factor) {
	speed = DEFAULT_SPEED * factor;
}

glm::mat4 Camera::getViewMatrix() {
	// Create translation and rotation matrices from camera position and axes.
	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -position.x;
	translation[3][1] = -position.y;
	translation[3][2] = -position.z;
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = xAxis.x;
	rotation[1][0] = xAxis.y;
	rotation[2][0] = xAxis.z;
	rotation[0][1] = yAxis.x;
	rotation[1][1] = yAxis.y;
	rotation[2][1] = yAxis.z;
	rotation[0][2] = -zAxis.x;
	rotation[1][2] = -zAxis.y;
	rotation[2][2] = -zAxis.z;

	// Return view matrix as translation and rotation product.
	return rotation * translation;
}

void Camera::update() {
	zAxis = glm::normalize(glm::vec3(
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	));
	xAxis = glm::normalize(glm::cross(zAxis, UP));
	yAxis = glm::normalize(glm::cross(xAxis, zAxis));
}