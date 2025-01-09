#pragma once
#include "res_resource_tag.h"

namespace res
{
	struct Vertex
	{
		glm::vec3 position = glm::vec3{ 0 };  // 0
		glm::vec3 normal = glm::vec3{ 0 };    // 1
		glm::vec2 uv = glm::vec2{ 0 };        // 2
		glm::vec3 tangent = glm::vec4{ 0 };   // 3
		glm::vec3 bitangent = glm::vec4{ 0 }; // 4
		glm::vec4 bones_weight = glm::vec4{0};// 5
		glm::vec4 color = glm::vec4{ 1 };	  // 6

		bool operator== (const Vertex& rhs) const;
	};

	struct Material
	{
		Tag diffuse;
		Tag specular;
		Tag ambient;
		Tag normal;
	};

	struct animation
	{
		std::string name;
		float duration = 0.f;
		float ticks_per_second = 0.f;
	};

	template<class T>
	struct animation_key_t
	{
		T value;
		float time;
	};

	using animation_pos_key = animation_key_t<glm::vec3>;
	using animation_rotate_key = animation_key_t<glm::quat>;
	using animation_scale_key = animation_key_t<glm::vec3>;

	struct animation_node
	{
		std::vector<animation_pos_key> pos_keys;
		std::vector<animation_rotate_key> rotate_keys;
		std::vector<animation_scale_key> scale_keys;
	};

	struct bone
	{
		std::unordered_map<std::string, animation_node> anim;
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
	struct bone_view
	{
		std::size_t begin = 0;
		std::size_t end = 0;
		res::Tag bones_indeces_txm;

		std::size_t get_offset() const { return begin; }
		std::size_t get_count() const { return end - begin; }
	};

	struct bone_container
	{
		std::vector<int32_t> bones_indeces; // vertex_count * max_bones_count_per_vertex
		//std::vector<bone_view> data;
		res::Tag bones_indeces_txm;
		glm::ivec2 original_size{ 0 };
	};

	struct meshes_conteiner
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<bone> bones; //?
		std::vector<glm::mat4> bones_matrices;
		std::vector<Material> materials;
		bone_container bones_data; //?
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

	struct node_hierarchy_view
	{
		std::vector<mesh_view> meshes;
		std::vector<node_hierarchy_view> children; //?
		std::string name;
		glm::mat4 mt{ 1.0 };//?
		glm::mat4 original{ 1.0 };//?
		std::unordered_map<std::string, res::animation_node> anim;
		int bone_id = -1;//?
	};

	struct model_presintation
	{
		meshes_conteiner data;
		std::vector<animation> animations;
		std::vector<mesh_view> meshes_views;//?
		node_hierarchy_view head;
	};
}