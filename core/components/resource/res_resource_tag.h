#pragma once
#include <glm/glm.hpp>
#include <string>
#include <format>

namespace res
{
	class Tag
	{
	public:
		struct hash {
			std::size_t operator() (const Tag& tag) const {
				return tag.get_hash();
			}
		};

		static const std::string_view default_protocol() { return "res"; }
		static Tag make(const std::string_view path) {
			return Tag{ default_protocol(), path };
		}

	public:
		explicit Tag(const std::string_view pref, const std::string_view path) 
		{
			full_ = std::vformat("{}://{}", std::make_format_args(pref, path));

			protocol_ = { 0, pref.length() };
			const std::size_t name_start_idx = full_.find_last_of("/");
			const std::size_t path_start_idx = full_.find_first_of("/") + 1;
			path_ = { path_start_idx + 1, name_start_idx - path_start_idx };
			name_ = { name_start_idx + 1, full_.length() - name_start_idx - 1 };
		}

		Tag() = default;
		Tag(const Tag&) = default;
		Tag(Tag&&) = default;
		Tag& operator= (const Tag&) = default;
		Tag& operator= (Tag&&) = default;

		bool is_valid() const { return !full_.empty(); }

		const std::string_view protocol() const { return std::string_view(full_.data() + protocol_.x, protocol_.y); }
		const std::string_view path() const { return std::string_view(full_.data() + path_.x, path_.y); }
		const std::string_view name() const { return std::string_view(full_.data() + name_.x, name_.y); }
		const std::string_view pure_name() const { auto result = name(); return result.substr(0, result.find_last_of('.'));  }
		const std::string_view get_full() const { return full_; }

		bool operator== (const Tag& tag) const { return get_hash() == tag.get_hash(); }
		std::size_t get_hash() const { return std::hash<std::string>{}(full_); }

	private:
		std::string full_;
		glm::ivec2 protocol_;
		glm::ivec2 path_;
		glm::ivec2 name_;
	};

	Tag operator+ (const Tag& a, const Tag& b);
}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<res::Tag>
	{
		std::size_t operator()(const res::Tag& tag) const
		{
			return tag.get_hash();
		}
	};

}
