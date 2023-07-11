#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
	glm::vec3 position;
	glm::vec3 xAxis;
	glm::vec3 yAxis;
	glm::vec3 zAxis;
	float yaw;
	float pitch;
	float speed;
	float sensitivity;
	float fovY;

	Camera();
	Camera(glm::vec3 position);
	Camera(glm::vec3 position, float yaw, float pitch);
	~Camera();

	void translate(glm::vec3 direction, float deltaTime);
	void rotate(float xOffset, float yOffset, bool constrainPitch = true);
	void zoom(float yOffset);
	void accelerate(float factor = 2.0f);
	glm::mat4 getViewMatrix();

private:
	void update();
};
#endif