#pragma once
#include <common.h>
#include "res_resource_base.h"

namespace res
{
	class Picture;

	struct Vertex
	{
		glm::vec3 position_;
		glm::vec3 normal_;
		glm::vec2 tex_uv_;
		glm::vec3 tangent_;
		glm::vec3 bitangent_;
		int bones_[4];
		float bones_weight_[4];
	};

	struct Material
	{
		Tag tag_;
		std::string type_;
	};

	struct Mesh
	{
		std::vector<Vertex> vertexes_;
		std::vector<unsigned int> indeces_;
		std::vector<Material> textures_;
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
