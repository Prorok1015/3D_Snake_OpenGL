#pragma once
#include <glm/fwd.hpp>
#include <assimp/scene.h>
#include <stack>

#include "res_mesh.hpp"
#include "res_resource_tag.h"

namespace res::loader {
    class model_loader
    {
    public:
        model_loader(const Tag& tag_)
            : tag(tag_)
        {
        }

        std::vector<Mesh> load();

        void process_node(aiNode* node, const aiScene* scene, res::node_hierarchy_view& hierarchy);

        Mesh copy_mesh(aiMesh* mesh, const aiScene* scene, res::node_hierarchy_view& hierarchy);

        Material copy_material(aiMaterial* mat);

        std::vector<res::Vertex> copy_vertices(aiMesh* mesh);

        std::vector<res::bone> copy_bones(std::vector<res::Vertex>& vertices, aiMesh* mesh, std::size_t base_vertex);

        std::vector<unsigned int> copy_indeces(aiMesh* mesh);

        Tag find_material_texture(aiMaterial* mat, aiTextureType type);

        glm::mat4 convert_to_glm(const aiMatrix4x4& transform) const {
            glm::mat4 local(1);
            local[0] = glm::vec4(transform.a1, transform.b1, transform.c1, transform.d1);
            local[1] = glm::vec4(transform.a2, transform.b2, transform.c2, transform.d2);
            local[2] = glm::vec4(transform.a3, transform.b3, transform.c3, transform.d3);
            local[3] = glm::vec4(transform.a4, transform.b4, transform.c4, transform.d4);
            return local;
        }

        void increase_txt_counter(const res::Tag& txt)
        {
            if (txt.is_valid()) {
                textures_counter[txt]++;
            }
        }

        void batch_meshes();
        void trace_node(const std::stack<aiNode*>& stack);

        res::node_hierarchy model_hierarchy;
        model_presintation model;

    private:
        Tag tag;

        std::unordered_map<std::string, std::size_t> bones_mapping;

        std::vector<Mesh> meshes;
        std::stack<glm::mat4> transform_stack;
        std::stack<aiNode*> nodes;
        std::unordered_map<res::Tag, int> textures_counter;
        std::unordered_map<aiMesh*, int> meshes_counter;
        std::size_t deep = 0;
    };
}
