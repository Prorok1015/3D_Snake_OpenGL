#pragma once
#include "../common/common.h"
#include "res_resource_base.h"

namespace resource
{
	class Texture;

	struct Vertex
	{
		glm::vec3 position_;
		glm::vec3 normal_;
		glm::vec3 tangent_;
		glm::vec3 bitangent_;
		glm::vec2 texture_position_;
		int bones_[4];
		float bones_weight_[4];
	};

	struct Material
	{
		std::shared_ptr<Texture> texture_ref_;
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
		Model(Tag tag);

		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Material> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

		const std::vector<Mesh>& get_meshes() const { return meshes_; }

	private:
		std::vector<Mesh> meshes_;
	};
}

namespace res = resource;