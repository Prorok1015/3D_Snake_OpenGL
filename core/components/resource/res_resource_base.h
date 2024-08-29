#pragma once
#include "res_resource_tag.h"

namespace res
{
	class Resource
	{
	public:
		Resource(const Tag& tag) : tag_(tag) {}

		bool operator== (const Tag& tag) const { return tag_ == tag; }

	protected:
		Tag tag_;
	};

	using RecourceRef = std::shared_ptr<Resource>;
}
