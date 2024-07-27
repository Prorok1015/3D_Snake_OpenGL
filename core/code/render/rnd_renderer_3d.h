#pragma once
#include "../common/common.h"
#include "../scene/model.h"
#include "../scene/mesh.h"
#include "rnd_vertex_array.h"

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
		std::shared_ptr<VertexArray> vertex_array;
		std::shared_ptr<VertexBuffer> vertex_buffer;

		std::shared_ptr<IndexBuffer> index_buffer;
	};
}

namespace rnd = render;