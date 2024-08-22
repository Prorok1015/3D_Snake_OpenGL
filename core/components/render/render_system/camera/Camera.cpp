#include "camera.h"

camera_static::camera_static(glm::vec3 pos, glm::ivec2 size, float fov_)
	:  transform(pos), fov(fov_), window_size(size), aspect((float)size.x / (float)size.y)
{
}

glm::mat4 camera_static::projection() const {
	return glm::perspective(glm::radians(fov), aspect, 0.1f, visible_distance);
}

glm::mat4 camera_static::view() const {
	return transform.to_matrix();
}

void camera_static::on_viewport_size_change(glm::ivec2 size)
{
	aspect = (float)size.x / (float)size.y;
	window_size = size;
}
