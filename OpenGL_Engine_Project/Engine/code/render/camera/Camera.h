#pragma once
#ifndef WINDOW_CAMERA_H_
#define WINDOW_CAMERA_H_

#include <glm/glm.hpp>

class Camera {
public:
	Camera(glm::vec3 position, float fov);
	void rotate(float x, float y, float z);
	glm::mat4 getProjection();
	glm::mat4 getView();

private:
	void updateVectors();

public:
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 position;
	float fov;
	glm::mat4 rotation;
};

#endif /* WINDOW_CAMERA_H_ */


