#pragma once
#include "common.h"

namespace scn
{
	struct viewport
	{
		glm::ivec2 center{ 0 };
		glm::ivec2 size{ 0 };

		viewport() = default;
		viewport(const glm::ivec4 vp)
			: center(vp.x, vp.y)
			, size(vp.z, vp.w) {}
	};

	struct camera_component
	{
		float fov = 90.f;
		float near_distance = 0.1f;
		float far_distance = 1000.f;
		viewport viewport;
	};

}