#include "camera.h"

camera_static::camera_static(glm::vec3 pos, glm::ivec2 size, float fov)
	:  transform(pos), fov_(fov), window_size(size), aspect((float)size.x / (float)size.y)
{
}

glm::mat4 camera_static::projection() const {
	return glm::perspective(fov_, aspect, 0.1f, distance);
}

glm::mat4 camera_static::view() const {
	return transform.get_view() * glm::translate(glm::mat4(1.0), glm::vec3(0, 0, y));
}

void camera_static::on_viewport_size_change(glm::ivec2 size)
{
	aspect = (float)size.x / (float)size.y;
	window_size = size;
}
