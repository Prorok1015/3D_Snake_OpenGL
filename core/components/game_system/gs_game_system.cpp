#include "gs_game_system.h"

#include <inp_input_manager.h>
#include <inp_input_system.h>

#include <enums.h>
#include <scn_primitives.h>
#include <scn_model.h>

#include <wnd_window_system.h>
#include <res_resource_system.h>

#include <rnd_render_system.h>
#include <ecs/ecs_common_system.h>

#include <timer.hpp>
#include <glm/gtc/random.hpp>

gs::GameSystem* p_game_system = nullptr;

gs::GameSystem& gs::get_system()
{
	ASSERT_MSG(p_game_system, "Game system is nullptr!");
	return *p_game_system;
}

template<typename R>
bool is_ready(std::future<R> const& f)
{
	return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

gs::GameSystem::GameSystem()
{
	window = wnd::get_system().get_active_window();
	input = std::make_shared<inp::input_manager>();
	inp::get_system().activate_manager(input);

	renderer = std::make_shared<renderer_3d>();
	rnd::get_system().activate_renderer(renderer);

	cubes_inst = ecs::create_entity();

	res::instance_object* inst = ecs::add_component(cubes_inst, res::instance_object{});
	inst->tpl = generate_cube().meshes.front();

	input->create_click_action(inp::KEYBOARD_BUTTONS::ESCAPE, [this](float) { window->shutdown(); });
}

gs::GameSystem::~GameSystem()
{
	rnd::get_system().deactivate_renderer(renderer);
	inp::get_system().deactivate_manager(input);
}

void gs::GameSystem::set_enable_input(bool enable)
{
	input->set_enabled(enable);
}

void gs::GameSystem::load_model(std::string_view path)
{
	future_model = std::make_shared<std::future<std::shared_ptr<res::Model>>>(res::get_system().require_resource_async<res::Model>(res::Tag::make(path)));
}

void gs::GameSystem::check_loaded_model()
{
	if (future_model && is_ready(*future_model)) {
		auto res = future_model->get();
		auto rand_pos = glm::diskRand(20.f);
		auto model = glm::translate(glm::mat4(1.0), glm::vec3{1.f});
		//m.model = glm::scale(m.model, glm::vec3{ 20 });

		ecs::entity obj = ecs::create_entity();
		ecs::add_component(obj, scn::model_comonent{ res->get_meshes(), res });
		ecs::add_component(obj, scn::transform_component{ model });
		ecs::add_component(obj, scn::is_render_component_flag{});

		renderer->scene_objects.push_back(obj);
		future_model = nullptr;
	}
}

void gs::GameSystem::reload_shaders()
{
	rnd::get_system().get_shader_manager().clear_cache();
}

void gs::GameSystem::add_cube_to_scene(float radius)
{
	glm::vec2 rand_pos = glm::diskRand(radius);
	res::instance_object* inst = ecs::get_component<res::instance_object>(cubes_inst);
	inst->worlds.push_back(glm::translate(glm::mat4{ 1.0 }, glm::vec3{ rand_pos.x, 1.f, rand_pos.y }));

	//static bool is_gen_cube = true;
	//scn::Model m = is_gen_cube ? generate_cube() : generate_sphere();
	//is_gen_cube = !is_gen_cube;
	//ecs::entity obj = ecs::create_entity();
	//ecs::add_component(obj, scn::model_comonent{ m.meshes });
	//ecs::add_component(obj, scn::transform_component{ m.model });
	//ecs::add_component(obj, scn::is_render_component_flag{});

	//renderer->scene_objects.push_back(obj);
}

void gs::GameSystem::remove_cube()
{
	if (renderer->scene_objects.empty()) {
		return;
	}

	ecs::remove_entity(renderer->scene_objects.back());

	renderer->scene_objects.pop_back();
}
