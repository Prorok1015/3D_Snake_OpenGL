#include "rnd_gl_vertex_array.h"
#include "glad/glad.h"

static GLenum ShaderDataTypeToOpenGLBaseType(render::driver::ShaderDataType type)
{
	using namespace render::driver;
	switch (type)
	{
	case ShaderDataType::Float:    return GL_FLOAT;
	case ShaderDataType::Float2:   return GL_FLOAT;
	case ShaderDataType::Float3:   return GL_FLOAT;
	case ShaderDataType::Float4:   return GL_FLOAT;
	case ShaderDataType::Mat3:     return GL_FLOAT;
	case ShaderDataType::Mat4:     return GL_FLOAT;
	case ShaderDataType::Int:      return GL_INT;
	case ShaderDataType::Int2:     return GL_INT;
	case ShaderDataType::Int3:     return GL_INT;
	case ShaderDataType::Int4:     return GL_INT;
	case ShaderDataType::Bool:     return GL_BOOL;
	}

	return 0;
}

render::driver::gl::vertex_array::vertex_array()
{
	glCreateVertexArrays(1, &m_RendererID);
}

render::driver::gl::vertex_array::~vertex_array()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void render::driver::gl::vertex_array::bind()
{
	glBindVertexArray(m_RendererID);
}

void render::driver::gl::vertex_array::unbind()
{
	glBindVertexArray(0);
}

void render::driver::gl::vertex_array::add_vertex_buffer(const std::shared_ptr<render::driver::buffer_interface>& vertexBuffer_in)
{
	bind();
	auto vertexBuffer = std::static_pointer_cast<gl::buffer>(vertexBuffer_in);
	vertexBuffer->bind();

	const auto& layout = vertexBuffer->get_layout();
	for (const auto& element : layout)
	{
		switch (element.Type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.get_component_count(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.get_stride(),
				(const void*)element.Offset);
			m_VertexBufferIndex++;
			
		} break;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		case ShaderDataType::Bool:
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribIPointer(m_VertexBufferIndex,
				element.get_component_count(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				layout.get_stride(),
				(const void*)element.Offset);
			m_VertexBufferIndex++;
			
		} break;
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		{
			uint8_t count = element.get_component_count();
			for (uint8_t i = 0; i < count; i++)
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					count,
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.get_stride(),
					(const void*)(element.Offset + sizeof(float) * count * i));
				glVertexAttribDivisor(m_VertexBufferIndex, 1);
				m_VertexBufferIndex++;
			}
			
		} break;

		}
	}

	m_VertexBuffers.push_back(vertexBuffer);
}

void render::driver::gl::vertex_array::remove_vertex_buffer(const std::shared_ptr<render::driver::buffer_interface>& vertexBuffer)
{
}

void render::driver::gl::vertex_array::set_index_buffer(const std::shared_ptr<render::driver::buffer_interface>& indexBuffer)
{
	bind();
	m_IndexBuffer = std::static_pointer_cast<gl::buffer>(indexBuffer);
	m_IndexBuffer->bind();
}