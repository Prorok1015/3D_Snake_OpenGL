#include "rnd_scene_shader_desc.h"
#include "res_system.h"
#include "rnd_shader_interface.h"
#include "res_resource_text_file.h"
#include "rnd_render_system.h"

std::vector<rnd::driver::shader_header> rnd::shader_scene_desc::load()
{
	auto vertexCode = res::get_system().require_resource<res::TextFile>(res::Tag::make("scene.vert"), true);
	auto fragmentCode = res::get_system().require_resource<res::TextFile>(res::Tag::make("scene.frag"), true);
	//auto geomCode = res::get_system().require_resource<res::TextFile>(res::Tag::make(name + ".geom"), true);
	std::vector<driver::shader_header> headers
	{
		driver::shader_header{.title = "scene.vert", .body = vertexCode->c_str(), .type = driver::shader_header::TYPE::VERTEX},
		driver::shader_header{.title = "scene.frag", .body = fragmentCode->c_str(), .type = driver::shader_header::TYPE::FRAGMENT},
	};

	return headers;
}

std::vector<rnd::driver::shader_header> rnd::shader_sky_desc::load()
{
	auto vertexCode = res::get_system().require_resource<res::TextFile>(res::Tag::make("sky.vert"), true);
	auto fragmentCode = res::get_system().require_resource<res::TextFile>(res::Tag::make("sky.frag"), true);
	//auto geomCode = res::get_system().require_resource<res::TextFile>(res::Tag::make(name + ".geom"), true);
	std::vector<driver::shader_header> headers
	{
		driver::shader_header{.title = "sky.vert", .body = vertexCode->c_str(), .type = driver::shader_header::TYPE::VERTEX},
		driver::shader_header{.title = "sky.frag", .body = fragmentCode->c_str(), .type = driver::shader_header::TYPE::FRAGMENT},
	};

	return headers;
}

std::vector<rnd::driver::shader_header> rnd::shader_scene_instance_desc::load()
{
	auto vertexCode = res::get_system().require_resource<res::TextFile>(res::Tag::make("scene_inst.vert"), true);
	auto fragmentCode = res::get_system().require_resource<res::TextFile>(res::Tag::make("scene_inst.frag"), true);
	//auto geomCode = res::get_system().require_resource<res::TextFile>(res::Tag::make(name + ".geom"), true);
	std::vector<driver::shader_header> headers
	{
		driver::shader_header{.title = "scene_inst.vert", .body = vertexCode->c_str(), .type = driver::shader_header::TYPE::VERTEX},
		driver::shader_header{.title = "scene_inst.frag", .body = fragmentCode->c_str(), .type = driver::shader_header::TYPE::FRAGMENT},
	};

	return headers;
}

void rnd::configure_render_pass(const shader_desc& decs, rnd::driver::shader_interface* shader)
{
    if (decs.tex0) {
        decs.tex0->bind(0);
    }

    if (decs.tex1) {
        decs.tex1->bind(1);
    }

    if (decs.tex2) {
        decs.tex2->bind(2);
    }

    if (decs.tex3) {
        decs.tex3->bind(3);
    }
}

void rnd::configure_render_pass(const shader_scene_desc& decs, rnd::driver::shader_interface* shader)
{
    shader->uniform("use_animation", decs.use_animation);
    shader->uniform("uWorldModel", decs.uWorldModel);
    shader->uniform("uWorldMeshMatr", decs.uWorldMeshMatr);
    shader->uniform("ambient", glm::vec3(1.0f, 0.5f, 0.31f));
    shader->uniform("shininess", 32.0f);

    if (decs.tex0) {
        decs.tex0->bind(0);
    }

    if (decs.tex1) {
        decs.tex1->bind(1);
    }

    if (decs.tex2) {
        decs.tex2->bind(2);
    }

    if (decs.tex3) {
        decs.tex3->bind(3);
    }
}