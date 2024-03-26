#pragma once
#include "../common/common.h"
#include "res_resource_base.h"

namespace resource
{
	class Shader : public Resource
	{
	public:
		Shader(const Tag& tag);

		const char* c_str() const { return body_.c_str(); }

	protected:
		std::string body_;
	};
}

namespace res = resource;