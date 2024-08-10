#pragma once
#include "../common/common.h"
#include <scene/scn_model.h>
#include <scene/scn_mesh.h>
//#include "rnd_vertex_array.h"
#include <rnd_gl_vertex_array.h>

namespace render
{
	class Renderer3d
	{
	public:
		Renderer3d();
		~Renderer3d();
		void init();
		void term();
		void draw(scene::Model& val);
		void draw(scene::Mesh& val);

	private:
		std::shared_ptr<render::driver::gl::vertex_array> vertex_array;
		std::shared_ptr<render::driver::gl::vertex_buffer> vertex_buffer;

		std::shared_ptr<render::driver::gl::index_buffer> index_buffer;
	};
}

namespace rnd = render;