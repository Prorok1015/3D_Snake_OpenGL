#include "rnd_gl_driver.h"
#include "open_gl_specific.h"
#include <glad/glad.h>
#include <rnd_gl_shader.h>
#include <rnd_gl_texture.h>
#include <rnd_gl_cubemap.h>
#include <rnd_gl_vertex_array.h>
#include <rnd_gl_buffer.h>
#include <rnd_gl_uniform_buffer.h>
#include <engine_log.h>

const GLenum gRenderModeToGLRenderMode[] =
{
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_FAN,
	GL_TRIANGLES_ADJACENCY,
	GL_TRIANGLE_STRIP_ADJACENCY,
	GL_LINES,
	GL_LINE_STRIP,
	GL_LINE_LOOP,
	GL_LINES_ADJACENCY,
	GL_LINE_STRIP_ADJACENCY,
	GL_POINTS,
};

const GLenum gShaderTypeToGLShaderType[] =
{
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER,
};

const GLint gTextureFilteringToGlFiltering[] =
{
	GL_NEAREST,
	GL_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR,
};

const GLint gTextureWrappingToGlWrapping[] =
{
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
};

const GLint gClearFlagsToGlClearFlags[] =
{
	GL_COLOR_BUFFER_BIT,
	GL_DEPTH_BUFFER_BIT,
};

const GLint gEnableFlagsToGlEnableFlags[] =
{
	GL_DEPTH_TEST,
	GL_DEPTH_TEST,
	GL_CULL_FACE,
};

const GLint gDepthFuncFlagsToGlDepthFuncFlags[] =
{
	GL_LESS,
	GL_LEQUAL
};

void rnd::driver::gl::driver::set_viewport(glm::ivec4 rect)
{
	if (viewport != rect) {
		egLOG("renderer/set_viewport", "viewport: '{},{}'", rect[2], rect[3]);
		viewport = rect;
		glViewport(rect[0], rect[1], rect[2], rect[3]);
	}
}

void rnd::driver::gl::driver::set_clear_color(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void rnd::driver::gl::driver::clear(CLEAR_FLAGS flags)
{
	glClear(gClearFlagsToGlClearFlags[(int)flags]);
}

void rnd::driver::gl::driver::set_activate_texture(int idx)
{
	// active proper texture unit before binding
	glActiveTexture(GL_TEXTURE0 + idx);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::driver::set_line_size(float size)
{
	glLineWidth(size);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::driver::set_point_size(float size)
{
	glPointSize(size);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::driver::draw_elements(RENDER_MODE render_mode, unsigned int vao, unsigned int count)
{
	glBindVertexArray(vao);
	CHECK_GL_ERROR();
	GLenum rm = gRenderModeToGLRenderMode[(int)render_mode];
	glDrawElements(rm, count, GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();
	glBindVertexArray(0);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::driver::draw_elements(RENDER_MODE render_mode, unsigned int count)
{
	GLenum rm = gRenderModeToGLRenderMode[(int)render_mode];
	glDrawElements(rm, count, GL_UNSIGNED_INT, 0);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::driver::enable(ENABLE_FLAGS flags)
{
	switch (flags)
	{
	case rnd::driver::ENABLE_FLAGS::DEPTH_TEST: 
	case rnd::driver::ENABLE_FLAGS::DEPTH_TEST_LEQUEL: 
		glDepthFunc(gDepthFuncFlagsToGlDepthFuncFlags[(int)flags]);
		CHECK_GL_ERROR();
		break;
	case rnd::driver::ENABLE_FLAGS::FACE_CULLING:
		glCullFace(GL_BACK);
		CHECK_GL_ERROR();
		break;
	}
	glEnable(gEnableFlagsToGlEnableFlags[(int)flags]);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::driver::disable(ENABLE_FLAGS flags)
{
	glDisable(gEnableFlagsToGlEnableFlags[(int)flags]);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::driver::unuse()
{
	glUseProgram(0);
	CHECK_GL_ERROR();
}

std::unique_ptr<rnd::driver::shader_interface> rnd::driver::gl::driver::create_shader(const std::vector<shader_header>& headers)
{
	std::vector<GLuint> shaders_ids;
	GLint success;
	GLchar infoLog[512];

	std::string dbgLogTitle;

	for (const auto& header : headers) {
		const GLchar* code = header.body.c_str();
		GLuint shader_id;

		shader_id = glCreateShader(gShaderTypeToGLShaderType[(int)header.type]);
		glShaderSource(shader_id, 1, &code, nullptr);
		glCompileShader(shader_id);
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader_id, 512, nullptr, infoLog);
			egLOG("shader/load", "{}", infoLog);
			continue;
		}
		if (dbgLogTitle.empty()) {
			dbgLogTitle = "'" + header.title + "'";
		} else {
			dbgLogTitle += ", '" + header.title + "'";
		}

		shaders_ids.push_back(shader_id);
	}

	// Shader Program
	GLuint id = glCreateProgram();
	for (auto shader_id : shaders_ids) {
		glAttachShader(id, shader_id);
	}

	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		for (auto shader_id : shaders_ids) {
			glDeleteShader(shader_id);
		}

		egLOG("shader/load", "{}", infoLog);
		return nullptr;
	}

	for (auto shader_id : shaders_ids) {
		glDeleteShader(shader_id);
	}

	egLOG("shader/load", "Success loading shader {}", dbgLogTitle);
	return std::make_unique<rnd::driver::gl::shader>(id);
}

std::unique_ptr<rnd::driver::texture_interface> rnd::driver::gl::driver::create_texture(const texture_header& header)
{

	GLsizei t_width = header.picture.width;
	GLsizei t_height = header.picture.height;
	GLenum format = 0;
	if (header.picture.channels == 1)
		format = GL_RED;
	else if (header.picture.channels == 3)
		format = GL_RGB;
	else if (header.picture.channels == 4)
		format = GL_RGBA;
	GLubyte* image_data = header.picture.data;
	GLuint texture;
	glGenTextures(1, &texture);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, texture);
	CHECK_GL_ERROR();
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gTextureFilteringToGlFiltering[(int)header.min]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gTextureFilteringToGlFiltering[(int)header.mag]);
	CHECK_GL_ERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gTextureWrappingToGlWrapping[(int)header.wrap]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gTextureWrappingToGlWrapping[(int)header.wrap]);
	CHECK_GL_ERROR();

	glTexImage2D(GL_TEXTURE_2D, 0, format, t_width, t_height, 0, format, GL_UNSIGNED_BYTE, image_data);
	CHECK_GL_ERROR();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
	CHECK_GL_ERROR();
	glGenerateMipmap(GL_TEXTURE_2D);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR();

	return std::make_unique<rnd::driver::gl::texture>(texture, t_width, t_height);
}

std::unique_ptr<rnd::driver::texture_interface> rnd::driver::gl::driver::create_texture(const cubmap_texture_header& header)
{
	GLuint texture;
	glGenTextures(1, &texture);
	CHECK_GL_ERROR();
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	CHECK_GL_ERROR();

	auto add = [](int direction, texture_header::data header)
	{
		auto width = header.width;
		auto height = header.height;
		auto data = header.data;
		GLenum format = 0;
		if (header.channels == 1)
			format = GL_RED;
		else if (header.channels == 3)
			format = GL_RGB;
		else if (header.channels == 4)
			format = GL_RGBA;
		glTexImage2D(direction, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	};

	add(GL_TEXTURE_CUBE_MAP_POSITIVE_X, header.right);
	add(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, header.left);
	add(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, header.top);
	add(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, header.bottom);
	add(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, header.back);
	add(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, header.front);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, gTextureFilteringToGlFiltering[(int)header.min]);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, gTextureFilteringToGlFiltering[(int)header.mag]);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, gTextureWrappingToGlWrapping[(int)header.wrap]);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, gTextureWrappingToGlWrapping[(int)header.wrap]);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, gTextureWrappingToGlWrapping[(int)header.wrap]);

	return std::make_unique<rnd::driver::gl::cubemap>(texture);
}

std::unique_ptr<rnd::driver::vertex_array_interface> rnd::driver::gl::driver::create_vertex_array()
{
	return std::make_unique<gl::vertex_array>();
}

std::unique_ptr<rnd::driver::buffer_interface> rnd::driver::gl::driver::create_buffer()
{
	return std::make_unique<gl::buffer>();
}

std::unique_ptr<rnd::driver::uniform_buffer_interface> rnd::driver::gl::driver::create_uniform_buffer(std::size_t size, std::size_t binding)
{
	return std::make_unique<gl::uniform_buffer>(size, binding);
}
