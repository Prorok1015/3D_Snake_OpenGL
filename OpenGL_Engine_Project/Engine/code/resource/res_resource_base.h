#pragma once
#include "res_resource_tag.h"

namespace resource
{
	class Resource
	{
	public:
		Resource(const Tag& tag) : tag_(tag) {}

		bool operator== (const Tag& tag) const { return tag_ == tag; }

	protected:
		Tag tag_;
	};

}

namespace res = resource;