#include "camera.h"
#include "../../windows/window.h"
#include "../../input/events.h"
#include <glm/ext.hpp>

Camera::Camera(glm::vec3 pos, float fov) : position(pos), fov(fov), rotation(1.0f) {
	update_vectors();
	Events::listeners[KEYBOARD::W] += [this] { position += front * Window::delta * speed; };
	Events::listeners[KEYBOARD::A] += [this] { position -= right * Window::delta * speed; };
	Events::listeners[KEYBOARD::S] += [this] { position -= front * Window::delta * speed; };
	Events::listeners[KEYBOARD::D] += [this] { position += right * Window::delta * speed; };
}

void Camera::update_vectors() {
	front = glm::vec3(rotation * glm::vec4(0, 0, -1, 1));
	right = glm::vec3(rotation * glm::vec4(1, 0, 0, 1));
	up = glm::vec3(rotation * glm::vec4(0, 1, 0, 1));
}

void Camera::rotate(float x, float y, float z) {
	rotation = glm::rotate(rotation, z, glm::vec3(0, 0, 1));
	rotation = glm::rotate(rotation, y, glm::vec3(0, 1, 0));
	rotation = glm::rotate(rotation, x, glm::vec3(1, 0, 0));

	update_vectors();
}

glm::mat4 Camera::projection() {
	float aspect = (float)Window::width / (float)Window::height;
	return glm::perspective(fov, aspect, 0.1f, 100.0f);
}

glm::mat4 Camera::view() {
	return glm::lookAt(position, position + front, up);
}