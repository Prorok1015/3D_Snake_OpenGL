#pragma once
#include <vector>
#include <string>

namespace rnd::driver
{
	enum class ShaderDataType
	{
		None, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t shader_data_type_size(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return sizeof(float);
		case ShaderDataType::Float2:   return sizeof(float) * 2;
		case ShaderDataType::Float3:   return sizeof(float) * 3;
		case ShaderDataType::Float4:   return sizeof(float) * 4;
		case ShaderDataType::Mat3:     return sizeof(float) * 3 * 3;
		case ShaderDataType::Mat4:     return sizeof(float) * 4 * 4;
		case ShaderDataType::Int:      return sizeof(int);
		case ShaderDataType::Int2:     return sizeof(int) * 2;
		case ShaderDataType::Int3:     return sizeof(int) * 3;
		case ShaderDataType::Int4:     return sizeof(int) * 4;
		case ShaderDataType::Bool:     return sizeof(bool);
		}

		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(shader_data_type_size(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t get_component_count() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3; // 3* float3
			case ShaderDataType::Mat4:    return 4; // 4* float4
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
			}

			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			calculate_offsets_and_stride();
		}

		uint32_t get_stride() const { return m_Stride; }
		const std::vector<BufferElement>& get_elements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void calculate_offsets_and_stride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};
}