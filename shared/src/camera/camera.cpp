#include "camera.hpp"

namespace {
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
	x(RIGHT),
	y(UP),
	z(FORWARD),
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
	// Calculate direction vectors.
	glm::vec3 zaxis = glm::normalize(position - (position + z));
	glm::vec3 xaxis = glm::normalize(glm::cross(UP, zaxis));
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);

	// Create translation matrix.
	glm::mat4 translation = glm::mat4(1.0f);
	translation[3][0] = -position.x;
	translation[3][1] = -position.y;
	translation[3][2] = -position.z;
	// Create rotation matrix.
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation[0][0] = xaxis.x;
	rotation[1][0] = xaxis.y;
	rotation[2][0] = xaxis.z;
	rotation[0][1] = yaxis.x;
	rotation[1][1] = yaxis.y;
	rotation[2][1] = yaxis.z;
	rotation[0][2] = zaxis.x;
	rotation[1][2] = zaxis.y;
	rotation[2][2] = zaxis.z;

	// Return view matrix from translation and rotation.
	return rotation * translation;
}

void Camera::update() {
	z = glm::normalize(glm::vec3(
		cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch))
	));
	x = glm::normalize(glm::cross(z, UP));
	y = glm::normalize(glm::cross(x, z));
}