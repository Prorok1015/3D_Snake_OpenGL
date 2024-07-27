#include "rnd_vertex_array.h"

static GLenum ShaderDataTypeToOpenGLBaseType(rnd::ShaderDataType type)
{
	using namespace rnd;
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

render::VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &m_RendererID);
	//m_RendererID = rnd::get_system().gen_vertex_buf();
}

render::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void render::VertexArray::bind() const
{
	glBindVertexArray(m_RendererID);
}

void render::VertexArray::unbind() const
{
	glBindVertexArray(0);
}

void render::VertexArray::add_vertex_buffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	bind();
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
			break;
		}
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
			break;
		}
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
			break;
		}
		
		}
	}

	m_VertexBuffers.push_back(vertexBuffer);
}

void render::VertexArray::set_index_buffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	bind();
	indexBuffer->bind();

	m_IndexBuffer = indexBuffer;
}