#include "camera.h"

Camera::Camera(glm::vec3 pos, glm::ivec2 size, float fov)
	:  transform(pos), fov_(fov), window_size(size), aspect((float)size.x / (float)size.y)
{
}

glm::mat4 Camera::projection() const {
	return glm::perspective(fov_, aspect, 0.1f, distance);
}

glm::mat4 Camera::view() const {
	return transform.get_view();
}

void Camera::on_viewport_size_change(glm::ivec2 size)
{
	aspect = (float)size.x / (float)size.y;
	window_size = size;
}
