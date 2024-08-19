#include "rnd_shader_manager.h"
#include <res_resource_system.h>
#include <res_resource_shader.h>


namespace glm
{
	decltype(auto) value_ptr(auto&& v)
	{
		return std::addressof(v);
	}
}

rnd::ShaderManager::ShaderManager(driver::driver_interface* ptr)
	: drv(ptr)
	, _matrices(drv->create_uniform_buffer(sizeof(GlobalUniform), 0))
{
}

rnd::ShaderManager::~ShaderManager()
{
}

rnd::Shader rnd::ShaderManager::use(std::string_view shader) const
{
	auto it = _cache.find(shader);
	if (it == _cache.end()) {
		auto new_shader = load(shader.data());
		new_shader->use();
		new_shader->uniform("texture_diffuse1", 0);
		auto& ref = _cache[shader] = std::move(new_shader);
		return Shader(ref.get());
	}
	it->second->use();
	return Shader(it->second.get());
}

void rnd::ShaderManager::unuse() const
{
	drv->unuse();
}

void rnd::ShaderManager::uniform(const std::string_view shader, const std::string_view field, glm::mat4 val) const
{
	auto sh = use(shader);
	sh.uniform(field, val);
	unuse();
}

void rnd::ShaderManager::uniform(const std::string_view shader, const std::string_view field, int val) const
{
	auto sh = use(shader);
	sh.uniform(field, val);
	unuse();
}

void rnd::ShaderManager::update_global_uniform(const GlobalUniform& val) const
{
	_matrices->set_data(glm::value_ptr(val.projection), sizeof(decltype(val.projection)), offsetof(GlobalUniform, projection));
	_matrices->set_data(glm::value_ptr(val.view), sizeof(decltype(val.view)), offsetof(GlobalUniform, view));
	_matrices->set_data(glm::value_ptr(val.time), sizeof(decltype(val.time)), offsetof(GlobalUniform, time));
}

std::unique_ptr<rnd::driver::shader_interface> rnd::ShaderManager::load(const std::string& name) const
{
	auto vertexCode = res::get_system().require_resource<res::Shader>(res::Tag::make(name + ".vert"), true);
	auto fragmentCode = res::get_system().require_resource<res::Shader>(res::Tag::make(name + ".frag"), true);
	//auto geomCode = res::get_system().require_resource<res::Shader>(res::Tag::make(name + ".geom"), true);
	std::vector<driver::shader_header> headers
	{
		driver::shader_header{.title = name + ".vert", .body = vertexCode->c_str(), .type = driver::shader_header::TYPE::VERTEX},
		driver::shader_header{.title = name + ".frag", .body = fragmentCode->c_str(), .type = driver::shader_header::TYPE::FRAGMENT},
	};

	return drv->create_shader(headers);
}
