#pragma once
#include "res_resource_tag.h"

namespace res
{
	struct Vertex
	{
		glm::vec3 position = glm::vec3{ 0 };
		glm::vec3 normal = glm::vec3{ 0 };
		glm::vec2 uv = glm::vec2{ 0 };
		glm::vec3 tangent = glm::vec4{ 0 };
		glm::vec3 bitangent = glm::vec4{ 0 };
		glm::ivec4 bones = glm::ivec4{ 0 };
		glm::vec4 bones_weight = glm::vec4{ 0 };

		bool operator== (const Vertex& rhs) const;
	};

	struct Material
	{
		Tag diffuse;
		Tag specular;
		Tag ambient;
		Tag normal;
	};

	struct bone
	{
		glm::mat4 offset{ 1.0 };
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<bone> bones;
		Material material;
	};
	//===========================================//
	struct meshes_conteiner
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<bone> bones;
		std::vector<Material> materials;
	};

	struct mesh_view
	{
		std::size_t vx_begin = 0;
		std::size_t vx_end = 0;
		std::size_t ind_begin = 0;
		std::size_t ind_end = 0;
		std::size_t bones_begin = 0;
		std::size_t bones_end = 0;
		std::size_t material_id = 0;

		std::size_t get_vertices_count() const { return vx_end - vx_begin; }
		std::size_t get_indices_count() const { return ind_end - ind_begin; }
		std::size_t get_bones_count() const { return bones_end - bones_begin; }
	};


	struct node_hierarchy
	{
		std::vector<Mesh> meshes;
		std::vector<node_hierarchy> children;
		std::string name;
		glm::mat4 mt{ 1.0 };
	};

	struct node_hierarchy_view
	{
		std::vector<mesh_view> meshes;
		std::vector<node_hierarchy_view> children;
		std::string name;
		glm::mat4 mt{ 1.0 };
	};

	struct model_presintation
	{
		meshes_conteiner data;
		std::vector<mesh_view> meshes_views;
		node_hierarchy_view head;
	};
}