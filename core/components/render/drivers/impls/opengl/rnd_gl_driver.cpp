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
#include <engine_assert.h>

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
	GL_DEPTH_TEST,
	GL_CULL_FACE,
};

const GLint gDepthFuncFlagsToGlDepthFuncFlags[] =
{
	GL_LESS,
	GL_LEQUAL,
	GL_EQUAL,
};

struct gl_type
{
	GLenum internal_format;
	GLenum format;
	GLenum data_type;
};

gl_type to_gl_type(rnd::driver::texture_header::TYPE type)
{
	GLenum format_internal = 0;
	GLenum format_2 = 0;
	GLenum data_type = GL_UNSIGNED_BYTE;
	switch (type)
	{
	case rnd::driver::texture_header::TYPE::R8:
	{
		format_internal = GL_R8;
		format_2 = GL_RED;
	}
	break;
	case rnd::driver::texture_header::TYPE::RGB8:
	{
		format_internal = GL_RGB8;
		format_2 = GL_RGB;
	}
	break;
	case rnd::driver::texture_header::TYPE::RGBA8:
	{
		format_internal = GL_RGBA8;
		format_2 = GL_RGBA;
	}
	break;
	case rnd::driver::texture_header::TYPE::R32I:
	{
		format_internal = GL_R32I;
		format_2 = GL_RED_INTEGER;
		data_type = GL_INT;
	}
	break;
	case rnd::driver::texture_header::TYPE::D24_S8:
	{
		format_internal = GL_DEPTH24_STENCIL8;
	}
	break;
	case rnd::driver::texture_header::TYPE::D32F:
	{
		format_internal = GL_DEPTH_COMPONENT32F;
	}
	break;
	case rnd::driver::texture_header::TYPE::D32F_S8:
	{
		format_internal = GL_DEPTH32F_STENCIL8;
	}
	break;
	case rnd::driver::texture_header::TYPE::D16:
	{
		format_internal = GL_DEPTH_COMPONENT16;
	}
	break;
	case rnd::driver::texture_header::TYPE::D24:
	{
		format_internal = GL_DEPTH_COMPONENT24;
	}
	break;
	case rnd::driver::texture_header::TYPE::D32:
	{
		format_internal = GL_DEPTH_COMPONENT32;
	}
	break;
	case rnd::driver::texture_header::TYPE::S1:
	{
		format_internal = GL_STENCIL_INDEX1;
	}
	break;
	case rnd::driver::texture_header::TYPE::S4:
	{
		format_internal = GL_STENCIL_INDEX4;
	}
	break;
	case rnd::driver::texture_header::TYPE::S8:
	{
		format_internal = GL_STENCIL_INDEX8;
	}
	break;

	}
	return { format_internal, format_2, data_type };
}

rnd::driver::gl::driver::driver()
{
	framebuffers.push({ 0, 0 });
}

rnd::driver::gl::driver::~driver()
{
}

void rnd::driver::gl::driver::attach_frame_buffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffers.top().first);
}

void rnd::driver::gl::driver::dettach_frame_buffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void rnd::driver::gl::driver::push_frame_buffer()
{
	GLuint framebuffer = 0;
	glCreateFramebuffers(1, &framebuffer);
	CHECK_GL_ERROR();
	framebuffers.push({ framebuffer, 0 });
}

void rnd::driver::gl::driver::pop_frame_buffer()
{
	if (framebuffers.size() == 1) {
		ASSERT_FAIL("Trying to pop backbuffer");
		return;
	}

	auto key = framebuffers.top();
	auto& [framebuffer, depth_stencil] = key;
	framebuffers.pop();
	glDeleteRenderbuffers(1, &depth_stencil);
	CHECK_GL_ERROR();
	glDeleteFramebuffers(1, &framebuffer);
	CHECK_GL_ERROR();

}

void rnd::driver::gl::driver::set_render_rarget(texture_interface* color, texture_interface* depth_stencil /* = nullptr*/)
{
	auto& [fb, ds_b] = framebuffers.top();
	if (color) {
		auto gl_color = static_cast<texture*>(color);
		glNamedFramebufferTexture(fb, GL_COLOR_ATTACHMENT0, gl_color->get_id(), 0);
		if (!depth_stencil) {
			glCreateRenderbuffers(1, &ds_b);
			glNamedRenderbufferStorage(ds_b, GL_DEPTH_COMPONENT32, gl_color->width(), gl_color->height());
			glNamedFramebufferRenderbuffer(fb, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ds_b);
		}
	}

	if (depth_stencil) {
		auto gl_depth_stencil = static_cast<texture*>(depth_stencil);
		texture_header::TYPE type = gl_depth_stencil->header.picture.channels;
		GLenum gl_type = 0;
		switch (type)
		{
		case rnd::driver::texture_header::TYPE::D32F_S8:
		case rnd::driver::texture_header::TYPE::D24_S8:
			gl_type = GL_DEPTH_STENCIL_ATTACHMENT;
			break;
		case rnd::driver::texture_header::TYPE::D32:
		case rnd::driver::texture_header::TYPE::D24:
		case rnd::driver::texture_header::TYPE::D16:
		case rnd::driver::texture_header::TYPE::D32F:
			gl_type = GL_DEPTH_ATTACHMENT;
			break;
		case rnd::driver::texture_header::TYPE::S1:
		case rnd::driver::texture_header::TYPE::S4:
		case rnd::driver::texture_header::TYPE::S8:
			gl_type = GL_STENCIL_ATTACHMENT;
			break;
		default:
			ASSERT_FAIL("Wrong depth_stencil render target type");
			break;
		}

		glNamedFramebufferTexture(fb, gl_type, gl_depth_stencil->get_id(), 0);
		CHECK_GL_ERROR();
	}
	attach_frame_buffer();
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::string errorString;
		switch (status) {
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			errorString = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			errorString = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			errorString = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			errorString = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			errorString = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			errorString = "GL_FRAMEBUFFER_UNSUPPORTED";
			break;

		default:
			errorString = "Unknown Framebuffer Error";
			break;
		}

		egLOG("Framebuffer is not complete: {0}", errorString);
		ASSERT_FAIL("ERROR");
	}
	dettach_frame_buffer();
}

void rnd::driver::gl::driver::set_viewport(glm::ivec4 rect)
{
	if (viewport != rect) {
		egLOG("renderer/set_viewport", "viewport: '{},{}'", rect[2], rect[3]);
		viewport = rect;
		glViewport(rect[0], rect[1], rect[2], rect[3]);
		CHECK_GL_ERROR();
	}
}

void rnd::driver::gl::driver::set_clear_color(glm::vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	CHECK_GL_ERROR();
}

void rnd::driver::gl::driver::clear(CLEAR_FLAGS flags)
{
	attach_frame_buffer();
	glClearDepth(1.0);
	glClear(gClearFlagsToGlClearFlags[(int)flags]);
	CHECK_GL_ERROR();
	dettach_frame_buffer();
}

void rnd::driver::gl::driver::clear(CLEAR_FLAGS flags, glm::vec4 color)
{
	set_clear_color(color);
	clear(flags);
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

void rnd::driver::gl::driver::draw_indeces(const std::unique_ptr<vertex_array_interface>& verteces, RENDER_MODE render_mode, unsigned int count, unsigned int base_vertex)
{
	GLenum rm = GL_TRIANGLES;
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffers.top().first);
	if (render_mode == RENDER_MODE::LINE_STRIP_ADJ) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		rm = gRenderModeToGLRenderMode[(int)render_mode];
	}

	verteces->bind();
	if (base_vertex > 0) {
		glDrawElementsBaseVertex(rm, count, GL_UNSIGNED_INT, 0, base_vertex);
		CHECK_GL_ERROR();
	} else {
		glDrawElements(rm, count, GL_UNSIGNED_INT, 0);
		CHECK_GL_ERROR();
	}
	verteces->unbind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void rnd::driver::gl::driver::draw_instanced_indeces(const std::unique_ptr<vertex_array_interface>& verteces, RENDER_MODE render_mode, unsigned int count, unsigned int instance_count, unsigned int offset)
{
	GLenum rm = GL_TRIANGLES;
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffers.top().first);
	if (render_mode == RENDER_MODE::LINE_STRIP_ADJ) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		rm = gRenderModeToGLRenderMode[(int)render_mode];
	}
	verteces->bind();
	glDrawElementsInstanced(rm, count, GL_UNSIGNED_INT, 0, instance_count);
	CHECK_GL_ERROR();
	verteces->unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void rnd::driver::gl::driver::enable(ENABLE_FLAGS flags)
{
	attach_frame_buffer();
	switch (flags)
	{
	case rnd::driver::ENABLE_FLAGS::DEPTH_MASK:
		glDepthMask(GL_TRUE);
		CHECK_GL_ERROR();
		break;
	case rnd::driver::ENABLE_FLAGS::DEPTH_TEST: 
	case rnd::driver::ENABLE_FLAGS::DEPTH_TEST_EQUEL:
	case rnd::driver::ENABLE_FLAGS::DEPTH_TEST_LEQUEL:
		glEnable(gEnableFlagsToGlEnableFlags[(int)flags]);
		CHECK_GL_ERROR();
		glDepthFunc(gDepthFuncFlagsToGlDepthFuncFlags[(int)flags]);
		CHECK_GL_ERROR();
		break;
	case rnd::driver::ENABLE_FLAGS::FACE_CULLING:
		glCullFace(GL_BACK);
		CHECK_GL_ERROR();
		glEnable(gEnableFlagsToGlEnableFlags[(int)flags]);
		CHECK_GL_ERROR();
		break;
	case ENABLE_FLAGS::COLOR_TEST:
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		CHECK_GL_ERROR();
		break;
	default:
		glEnable(gEnableFlagsToGlEnableFlags[(int)flags]);
		CHECK_GL_ERROR();
	}
	dettach_frame_buffer();
}

void rnd::driver::gl::driver::disable(ENABLE_FLAGS flags)
{
	attach_frame_buffer();
	switch (flags)
	{
	case rnd::driver::ENABLE_FLAGS::DEPTH_MASK:
		glDepthMask(GL_FALSE);
		CHECK_GL_ERROR();
		break;
	case ENABLE_FLAGS::COLOR_TEST:
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		return;
		break;
	default:
		glDisable(gEnableFlagsToGlEnableFlags[(int)flags]);
		CHECK_GL_ERROR();
		break;
	}
	dettach_frame_buffer();
}

void rnd::driver::gl::driver::unuse()
{
	glBindTextureUnit(0, 0);
	glBindTextureUnit(1, 0);
	glBindTextureUnit(2, 0);
	glBindTextureUnit(3, 0);
	glBindTextureUnit(4, 0);
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
			egLOG("shader/load", "Error compiling shader '{0}' with log: {1}", header.title, infoLog);
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

		egLOG("shader/load", "Error Linking shaders {}", dbgLogTitle);
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

	GLint maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

	ASSERT_MSG(t_height < maxTextureSize && t_width < maxTextureSize, "Texture size is too big");

	GLubyte* image_data = header.picture.data;
	GLuint texture;
	glCreateTextures(GL_TEXTURE_2D, 1, &texture);
	CHECK_GL_ERROR();

	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, gTextureFilteringToGlFiltering[(int)header.min]);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, gTextureFilteringToGlFiltering[(int)header.mag]);
	CHECK_GL_ERROR();

	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, gTextureWrappingToGlWrapping[(int)header.wrap]);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, gTextureWrappingToGlWrapping[(int)header.wrap]);
	CHECK_GL_ERROR();
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	CHECK_GL_ERROR();
	
	auto [format_internal, format_2, data_type] = to_gl_type(header.picture.channels);
	glTextureStorage2D(texture, 1, format_internal, t_width, t_height);
	CHECK_GL_ERROR();

	GLint result = 0;
	glGetTextureParameteriv(texture, GL_TEXTURE_IMMUTABLE_FORMAT, &result);
	if (result != GL_TRUE) {
		egLOG("texture/create", "Broken tex immutable format");
	}

	if (image_data) {
		glTextureSubImage2D(texture, 0, 0, 0, t_width, t_height, format_2, data_type, image_data);
		CHECK_GL_ERROR();
	}

	//glTextureParameteri(texture, GL_TEXTURE_MAX_LEVEL, 4);
	//CHECK_GL_ERROR();
	//glGenerateTextureMipmap(texture);
	//CHECK_GL_ERROR();

	return std::make_unique<rnd::driver::gl::texture>(texture, header);
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
		if (header.channels == rnd::driver::texture_header::TYPE::R8) {
			format = GL_RED;
		}
		else if (header.channels == rnd::driver::texture_header::TYPE::RGB8) {
			format = GL_RGB;
		}
		else if (header.channels == rnd::driver::texture_header::TYPE::RGBA8) {
			format = GL_RGBA;
		}
		else if (header.channels == rnd::driver::texture_header::TYPE::R32I) {
			format = GL_R32I;
		}
		glTexImage2D(direction, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	};

	add(GL_TEXTURE_CUBE_MAP_POSITIVE_X, header.right);
	CHECK_GL_ERROR();
	add(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, header.left);
	CHECK_GL_ERROR();
	add(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, header.top);
	CHECK_GL_ERROR();
	add(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, header.bottom);
	CHECK_GL_ERROR();
	add(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, header.back);
	CHECK_GL_ERROR();
	add(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, header.front);
	CHECK_GL_ERROR();

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, gTextureFilteringToGlFiltering[(int)header.min]);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, gTextureFilteringToGlFiltering[(int)header.mag]);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, gTextureWrappingToGlWrapping[(int)header.wrap]);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, gTextureWrappingToGlWrapping[(int)header.wrap]);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, gTextureWrappingToGlWrapping[(int)header.wrap]);

	return std::make_unique<rnd::driver::gl::texture>(texture, texture_header{}/*TODO*/);
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
