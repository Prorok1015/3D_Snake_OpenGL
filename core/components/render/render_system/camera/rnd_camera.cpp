#include "rnd_camera.h"

rnd::camera::camera(glm::vec3 pos, glm::ivec4 viewport, float fov_)
	:  fov(fov_), viewport_offset(viewport[0], viewport[1]), viewport_size(viewport[2], viewport[3])
{
	world = glm::translate(world, pos);
}

glm::mat4 rnd::camera::projection() const {
	return glm::perspective(glm::radians(fov), get_aspect(), MIN_VISIBLE_DISTANCE, get_visible_distance());
}

glm::mat4 rnd::camera::view() const {
	return glm::inverse(world);
}

void rnd::camera::set_viewport(glm::ivec2 offset, glm::ivec2 size)
{
	viewport_offset = offset;
	viewport_size = size;
}
