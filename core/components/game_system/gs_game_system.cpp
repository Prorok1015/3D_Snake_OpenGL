#include "gs_game_system.h"

#include <inp_input_manager.h>
#include <inp_input_system.h>

#include <enums.h>
#include <scn_primitives.h>
#include <scn_model.h>

#include <wnd_window_system.h>
#include "res_system.h"

#include <rnd_render_system.h>
#include <ecs_common_system.h>

#include <timer.hpp>
#include <glm/gtc/random.hpp>
#include "ecs_system.h"
#include "scn_camera_component.hpp"
#include "scn_transform_system.h"
#include "scn_material_component.hpp"
#include "ecs_component.h"

gs::GameSystem* p_game_system = nullptr;
extern int gMaxTexture2DSize;

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
	//input = std::make_shared<inp::input_manager>();
	ecs_input = std::make_shared<ecs::flow_input_manager>();
	//inp::get_system().activate_manager(input);
	inp::get_system().activate_manager(ecs_input);

	renderer = std::make_shared<scn::renderer_3d>();
	rnd::get_system().activate_renderer(renderer);

	//input->create_click_action(inp::KEYBOARD_BUTTONS::ESCAPE, [this](float) { window->shutdown(); });

	ecs::systems.push_back(scn::ecs_process_update_camera_matrix);
	ecs::systems.push_back(scn::update_transform_system);
	ecs::systems.push_back(scn::update_animation_system);
}

gs::GameSystem::~GameSystem()
{
	rnd::get_system().deactivate_renderer(renderer);
	//inp::get_system().deactivate_manager(input);
}

void gs::GameSystem::set_enable_input(bool enable)
{
	//input->set_enabled(enable);
}

void gs::GameSystem::load_model(std::string_view path)
{
	future_model = std::make_shared<std::future<std::shared_ptr<res::Model>>>(res::get_system().require_resource_async<res::Model>(res::Tag::make(path)));
}

void ensure_ecs_material(ecs::entity material, const res::Material& mlt)
{
	ecs::add_component(material, scn::name_component{ .name = mlt.name });
	scn::base_material_component base_mlt;
	if (mlt.is_state(res::Material::ALBEDO_COLOR)) {
		base_mlt.albedo = mlt.diffuse_color;
	}
	if (mlt.is_state(res::Material::SPECULAR_COLOR)) {
		base_mlt.specular = mlt.specular_color;
	}
	if (mlt.is_state(res::Material::AMBIENT_COLOR)) {
		base_mlt.ambient = mlt.ambient_color;
	}
	if (mlt.is_state(res::Material::EMISSIVE_COLOR)) {
		base_mlt.emissive = mlt.emissive_color;
	}
	if (mlt.is_state(res::Material::TRANSPARENT_COLOR)) {
		base_mlt.transparent = mlt.transparent_color;
	}
	if (mlt.is_state(res::Material::REFLECTIVE_COLOR)) {
		base_mlt.reflective = mlt.reflective_color;
	}
	if (mlt.is_state(res::Material::OPACITY)) {
		base_mlt.opacity = mlt.opacity;
	}
	if (mlt.is_state(res::Material::REFLECTIVITY)) {
		base_mlt.reflectivity = mlt.reflectivity;
	}
	if (mlt.is_state(res::Material::REFRACTI)) {
		base_mlt.refracti = mlt.refracti;
	}
	if (mlt.is_state(res::Material::SHININESS)) {
		base_mlt.shininess = mlt.shininess;
	}
	if (mlt.is_state(res::Material::SHININESS_STRENGTH)) {
		base_mlt.shininess_strength = mlt.shininess_strength;
	}
	ecs::add_component(material, base_mlt);
	if (mlt.is_state(res::Material::ALBEDO_TXM)) {
		ecs::add_component(material, scn::albedo_map_component{ .txm = mlt.get_txm(res::Material::ALBEDO_TXM) });
	}
	if (mlt.is_state(res::Material::NORMALS_TXM)) {
		ecs::add_component(material, scn::normal_map_component{ .txm = mlt.get_txm(res::Material::NORMALS_TXM) });
	}
	if (mlt.is_state(res::Material::SPECULAR_TXM)) {
		ecs::add_component(material, scn::specular_map_component{ .txm = mlt.get_txm(res::Material::SPECULAR_TXM) });
	}
	if (mlt.is_state(res::Material::AMBIENT_OCCLUSION_TXM)) {
		ecs::add_component(material, scn::ao_map_component{ .txm = mlt.get_txm(res::Material::AMBIENT_OCCLUSION_TXM) });
	}
}

void ensure_ecs_node(ecs::entity ent, const res::node_hierarchy_view& node, const res::meshes_conteiner& data, std::unordered_map<int, ecs::entity> material_mapping)
{
	ecs::add_component(ent, scn::is_render_component_flag{});
	ecs::add_component(ent, scn::name_component{ .name = node.name });
	ecs::add_component(ent, scn::transform_component{ .local = node.mt });

	if (!node.anim.empty()) {
		ecs::add_component(ent, scn::keyframes_component{ .keyframes = node.anim });
	}

	if (node.bone_id != -1) {
		auto& bone = data.bones[node.bone_id];
		ecs::add_component(ent, scn::bone_component{ .offset = bone.offset });
		if (auto* key = ecs::get_component<scn::keyframes_component>(ent)) {
			key->keyframes = bone.anim;
			egLOG("model/fill_components", "There is double animation on the node '{0}'", node.name);
		} else {
			ecs::add_component(ent, scn::keyframes_component{ .keyframes = bone.anim });
		}
	}

	std::vector<ecs::entity> children;
	for (auto& mesh : node.meshes)
	{
		ecs::entity mesh_ent = ecs::create_entity();
		children.push_back(mesh_ent);
		ecs::add_component(mesh_ent, scn::parent_component{ .parent = ent });
		ecs::add_component(mesh_ent, scn::mesh_component{ .mesh = mesh });
		ecs::add_component(mesh_ent, scn::transform_component{});
		ecs::add_component(mesh_ent, scn::is_render_component_flag{});
		ecs::add_component(mesh_ent, scn::material_link_component{ .material = material_mapping[mesh.material_id]});
	}

	for (auto& child : node.children)
	{
		ecs::entity child_ent = ecs::create_entity();
		children.push_back(child_ent);
		ecs::add_component(child_ent, scn::parent_component{ .parent = ent });
		ensure_ecs_node(child_ent, child, data, material_mapping);
	}

	if (!children.empty()) {
		ecs::add_component(ent, scn::children_component{ .children = children });
	}
}

// TODO: remove
void gs::GameSystem::end_ecs_frame()
{
	for (auto ent : ecs::filter<ecs::input_changed_event_component>())
	{
		ecs::remove_entity(ent);
	}

	inp::get_system().mouse.clear_scroll();
}

void gs::GameSystem::check_loaded_model()
{
	if (future_model && is_ready(*future_model)) {
		auto res = future_model->get();
		auto& pres = res->get_model_pres();
		auto rand_pos = glm::ballRand(20.f);
		auto model = glm::translate(glm::mat4(1.0), glm::vec3{ 0 });
		model = glm::scale(model, glm::vec3{ 1 });

		ecs::entity obj = ecs::create_entity();
		if (auto& bones_data = res->get_model_pres().data.bones_data.bones_indeces; !bones_data.empty()) { 
			// TODO: create texture only for model desc
			res::Tag txm = res::Tag("memory", "__bones_indeces_" + std::to_string(obj.index));
			auto& last_bone_view = res->get_model_pres().data.bones_data;
			last_bone_view.bones_indeces_txm = txm;

			glm::ivec2 size{ res->get_model_pres().data.vertices.size(), 0 };
			size.y = bones_data.size() / size.x;

			const auto& origin_width = size.x;
			const auto& origin_height = size.y;
			last_bone_view.original_size = { 1, origin_height };

			if (origin_width > gMaxTexture2DSize)
			{
				int real_width = origin_width;
				int pices_count = 1;
				while (real_width > gMaxTexture2DSize)
				{
					if (real_width % 2 != 0) {
						++real_width;
					}
					real_width /= 2;
					pices_count *= 2;
				}

				int add = (real_width * pices_count) - origin_width;

				bones_data.reserve(bones_data.size() + (add * origin_height));
				for (int i = 0; i < add; ++i)
				{
					for (int j = 1; j <= origin_height; ++j) {
						int offset = j * origin_width;
						bones_data.insert(bones_data.begin() + offset, -1);
					}
				}

				last_bone_view.original_size = { pices_count, origin_height };
				size.x = real_width;
				size.y = bones_data.size() / real_width;
			}
			

			rnd::driver::texture_header header;
			header.picture.data = (unsigned char*)bones_data.data();
			header.picture.channels = rnd::driver::texture_header::TYPE::R32I;

			header.picture.width = size.x;
			header.picture.height = size.y;

			header.wrap = rnd::driver::texture_header::WRAPPING::CLAMP_TO_EDGE;
			header.min = rnd::driver::texture_header::FILTERING::NEAREST;
			header.mag = rnd::driver::texture_header::FILTERING::NEAREST;

			rnd::get_system().get_texture_manager().generate_texture(txm, header);
		}

		pres.head.mt = model * pres.head.mt;
		std::unordered_map<int, ecs::entity> material_mapping;
		auto anchors = ecs::filter<scn::scene_anchor_component>();
		ecs::entity world_anchor;
		if (anchors.empty()) {
			world_anchor = ecs::create_entity();
			ecs::add_component(world_anchor, scn::scene_anchor_component{});
			ecs::add_component(world_anchor, scn::name_component{ .name = "Anchor" });
			anchors.push_back(world_anchor);
		} else {
			world_anchor = anchors.front();
		}
		decltype(scn::children_component::children) children;
		if (auto* kids = ecs::get_component<scn::children_component>(world_anchor)) {
			children = kids->children;
			ecs::remove_component<scn::children_component>(world_anchor);
		}

		for (int i = 0; i < pres.data.materials.size(); ++i) {
			auto material = ecs::create_entity();
			ensure_ecs_material(material, pres.data.materials[i]);
			material_mapping[i] = material;
		}

		children.push_back(obj);
		ecs::add_component(anchors.front(), scn::children_component{ .children = children });

		ecs::add_component(obj, scn::parent_component{ .parent = anchors.front() });
		ecs::add_component(obj, scn::model_root_component{ .data = pres.data });
		if (!pres.animations.empty()) {
			ecs::add_component(obj, scn::animations_component{ .animations = pres.animations });
		}		

		ensure_ecs_node(obj, pres.head, pres.data, material_mapping);

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
