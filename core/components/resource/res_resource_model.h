#pragma once
#include <common.h>
#include "res_resource_base.h"

namespace res
{
	class Picture;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		int bones[4];
		float bones_weight[4];
	};

	struct Material
	{
		Tag diffuse;
		Tag specular;
		Tag ambient;
		Tag normal;
	};

	struct Mesh
	{
		std::vector<Vertex> vertexes;
		std::vector<unsigned int> indeces;
		Material material;
	};

	class Model : public Resource
	{
	public:
		Model(const Tag& tag);

		const std::vector<Mesh>& get_meshes() const { return meshes_; }

	private:
		std::vector<Mesh> meshes_;
	};
}
