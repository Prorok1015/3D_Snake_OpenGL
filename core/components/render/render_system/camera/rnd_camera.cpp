#include "rnd_camera.h"
#include <ecs/ecs_common_system.h>
#include "scn_model.h"

rnd::camera::camera(glm::vec3 pos, glm::ivec4 viewport, float fov_)
	:  fov(fov_), viewport_offset(viewport[0], viewport[1]), viewport_size(viewport[2], viewport[3])
{
	world = glm::translate(world, pos);
	ecs_entity = ecs::create_entity();
	ecs::add_component(ecs_entity, camera_accessor_component{ this });
	ecs::add_component(ecs_entity, camera_component{ .fov = fov_, .viewport = viewport});
	ecs::add_component(ecs_entity, scn::transform_component{ .local = world });
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

glm::mat4 rnd::make_projection(camera_component& camera)
{
	return glm::perspective(glm::radians(camera.fov), make_aspect(camera), MIN_VISIBLE_DISTANCE, camera.view_distance);
}

float rnd::make_aspect(camera_component& camera)
{
	return (float)camera.viewport.size.x / (float)camera.viewport.size.y;
}