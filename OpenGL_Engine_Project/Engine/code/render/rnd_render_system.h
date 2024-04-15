#pragma once
#include "../common/common.h"
#include "texture/rnd_texture_manager.h"
#include "shader/rnd_shader_manager.h"

namespace render
{
	class RenderSystem
	{
	public:
		RenderSystem() = default;
		~RenderSystem() = default;
		RenderSystem(const RenderSystem&) = delete;
		RenderSystem(RenderSystem&&) = delete;
		RenderSystem& operator= (const RenderSystem&) = delete;
		RenderSystem& operator= (RenderSystem&&) = delete;

		const rnd::TextureManager& get_txr_manager() const { return txrManager; }
		const rnd::ShaderManager& get_sh_manager() const { return shManager; }

		void viewport(glm::ivec4 rect) const {
			glViewport(rect[0], rect[1], rect[2], rect[3]);
		}

		void enable(int flag) const {
			glEnable(flag);
		}
		void clear_color(const glm::vec4& val) const {
			glClearColor(val.r, val.g, val.b, val.a);
		}
		void clear(int flag) const { glClear(flag); }
		void activate_texture_unit(int i) const {
			 // active proper texture unit before binding
			glActiveTexture(GL_TEXTURE0 + i);
			CHECK_GL_ERROR();
		}

		void reset_texture() const {
			glActiveTexture(GL_TEXTURE0);
		}

		void draw_elements(unsigned int vao, unsigned int count) const {
			glBindVertexArray(vao);
			CHECK_GL_ERROR();
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
			CHECK_GL_ERROR();
			glBindVertexArray(0);
			CHECK_GL_ERROR();
		}

		unsigned int gen_vertex_buf(int count = 1) const {
			unsigned int vao;
			glGenVertexArrays(count, &vao);
			CHECK_GL_ERROR();
			return vao;
		}

		void del_vertex_buf(unsigned int vao, int count = 1) const {
			glDeleteVertexArrays(count, &vao);
		}

		unsigned int gen_buf(int count = 1) const {
			unsigned int buf;
			glGenBuffers(count, &buf);
			CHECK_GL_ERROR();
			return buf;
		}

		void del_buf(unsigned int buf, int count = 1) const {
			glDeleteBuffers(count, &buf);
		}

		void bind_vertex_array(unsigned int vao) const {
			glBindVertexArray(vao);
			CHECK_GL_ERROR();
		}

		void bind_buffer(unsigned int buf, int type) const {
			glBindBuffer(type, buf);
		}

		void buffer_data(int flag, int size, void* data) const {
			glBufferData(flag, size, data, GL_STATIC_DRAW);
		}

		void vertex_attribute_pointer(int idx, int count, int type, int size, void* offset) const {

			glEnableVertexAttribArray(idx);
			glVertexAttribPointer(idx, count, type, GL_FALSE, size, offset);
		}

		void vertex_attribute_pointeri(int idx, int count, int type, int size, void* offset) const {

			glEnableVertexAttribArray(idx);
			glVertexAttribIPointer(idx, count, type, size, offset);
		}

	private:
		rnd::TextureManager txrManager;
		rnd::ShaderManager shManager;
	};

	RenderSystem& get_system();
}

namespace rnd = render;