#pragma once
#include "common.h"
#include "ecs_entity.h"

namespace scn
{
	struct material_link_component
	{
		ecs::entity material;
	};

	struct base_material_component
	{
		ds::color albedo = ds::color(1.0f); 
		ds::color specular = ds::color(1.0f);
		ds::color ambient = ds::color(1.0f);
		ds::color emissive = ds::color(1.0f);
		ds::color transparent = ds::color(1.0f);
		ds::color reflective = ds::color(1.0f);
		float opacity = 1.0f;
		float reflectivity = 0.0;// 0.0 -> 1.0
		float refracti = 0.0;
		float shininess = 0.0;// 0.0 -> 1000
		float shininess_strength = 0.0;// 0.0 -> 1.0
	};

	struct albedo_map_component {	res::Tag txm; };
	struct normal_map_component { res::Tag txm; };
	struct roughness_map_component { res::Tag txm; };
	struct metallic_map_component { res::Tag txm; };
	struct ao_map_component { res::Tag txm; };
	struct height_map_component { res::Tag txm; };
	struct specular_map_component { res::Tag txm; };
	struct glossiness_map_component { res::Tag txm; };
	struct emissive_map_component { res::Tag txm; };
	struct opacity_map_component { res::Tag txm; };

}