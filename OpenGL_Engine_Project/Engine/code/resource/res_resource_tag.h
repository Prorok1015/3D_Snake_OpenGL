#pragma once
#include "../common/common.h"

namespace resource
{
	class Tag
	{
	public:
		struct hash {
			std::size_t operator() (const Tag& tag) const {
				return tag.get_hash();
			}
		};

		static constexpr std::string get_default_pref() { return "res"; }
		static Tag make(std::string path) {
			return Tag{ get_default_pref(), std::move(path) };
		}

	public:
		explicit Tag(const std::string& pref, const std::string& path) 
		{
			full_path_ = pref + "://" + path;
			pref_ = std::string_view(full_path_.data(), pref.length());
			std::size_t name_start_idx = full_path_.find_last_of("/");
			std::size_t path_start_idx = full_path_.find_first_of("/") + 1;
			path_ = std::string_view(full_path_.data() + path_start_idx + 1, name_start_idx - path_start_idx);
			name_ = std::string_view(full_path_.data() + name_start_idx + 1, full_path_.length() - name_start_idx - 1);
		}

		Tag(const Tag&) = default;
		Tag(Tag&&) = default;
		Tag& operator= (const Tag&) = default;
		Tag& operator= (Tag&&) = default;

		const std::string_view get_pref() const { return pref_; }
		const std::string_view get_path() const { return path_; }
		const std::string_view get_name() const { return name_; }
		const std::string_view get_full() const { return full_path_; }
		bool operator== (const Tag& tag) const { return get_hash() == tag.get_hash(); }
		std::size_t get_hash() const { return std::hash<std::string>{}(full_path_); }

	private:
		std::string full_path_;
		std::string_view pref_;
		std::string_view path_;
		std::string_view name_;
	};

	class RelativeTag
	{
	public:
		explicit RelativeTag(Tag a, Tag b) : tags_{a, b} {}
		explicit RelativeTag(Tag a) : tags_{a} {}
		RelativeTag(const RelativeTag&) = default;
		RelativeTag(RelativeTag&&) = default;
		RelativeTag& operator=(const RelativeTag&) = default;
		RelativeTag& operator=(RelativeTag&&) = default;

		RelativeTag& append(const RelativeTag& rt) 
		{ 
			tags_.reserve(tags_.capacity() + rt.tags_.size());
			for (const auto& tag : rt.tags_) {
				tags_.push_back(tag);
			}
			return *this;
		}

		RelativeTag& append(const Tag& tag) 
		{ 
			tags_.push_back(tag);
			return *this;
		}

		const std::vector<Tag>& tags() const { return tags_; }

		operator Tag()
		{
			if (tags_.empty()) {
				ASSERT_FAIL("relative tag is empty");
			}

			std::string path;
			for (const auto& t : tags_) {
				path += t.get_path();
			}

			path += tags_.back().get_name();

			return Tag::make(path);
		}

	private:
		std::vector<Tag> tags_;
	};

	//RelativeTag operator+ (const Tag& a, const Tag& b);
	Tag operator+ (const Tag& a, const Tag& b);
	RelativeTag operator+ (RelativeTag a, Tag b);
	RelativeTag operator+ (RelativeTag a, RelativeTag b);
}

namespace res = resource;