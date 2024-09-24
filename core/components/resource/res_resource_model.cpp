#include "common.h"
#include "res_resource_model.h"
#include "res_resource_texture.h"
#include "res_resource_system.h"
#include <engine_log.h>
 

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stack>

namespace res::loader {
    class model_loader
    {
    public:
        model_loader(const Tag& tag_) 
            : tag(tag_)
        {
        }

        std::vector<Mesh> load()
        {
            // read file via ASSIMP
            Assimp::Importer importer;
            const std::string full_path = ResourceSystem::get_absolut_path(tag);
            egLOG("model/load", "Start loading model '{}' by absolut path:\n\t '{}'", tag.get_full(), full_path);

            const aiScene* scene = importer.ReadFile(full_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
            // check for errors
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                egLOG("scene/model/load", "ERROR::ASSIMP::{}", importer.GetErrorString());
                return {};
            }

            aiNode* Root = scene->mRootNode;
            if (tag.name().find(".gltf") != std::string::npos) {
                for (int i = 0; i < 2; ++i) {
                    if (Root->mNumChildren > 0) {
                        Root = Root->mChildren[0];
                    }
                }
            }

            // process ASSIMP's root node recursively
            process_node(Root, scene);
            egLOG("load/deep", "max deep: {}", deep);
            egLOG("load/meshes", "meshes: {}", meshes.size());
            for (auto& [txt, count] : textures_counter)
            {
                egLOG("load/textures", "texture: {}, use: {}", txt.get_full(), count);
            }

            for (auto& [mesh, count] : meshes_counter)
            {
                egLOG("load/meshes", "mesh: {}, use: {}", (void*)mesh, count);
            }

            for (auto& mesh : meshes)
            {
                egLOG("load/meshes", "mesh: vx - {}   \tind - {},   \tmaterial: {}, {}, {}, {}", mesh.vertices.size(), mesh.indices.size(),
                    mesh.material.diffuse.name(), mesh.material.ambient.name(), mesh.material.normal.name(), mesh.material.specular.name());
            }

            batch_meshes();
            return meshes;
        }

        void print(const std::stack<aiNode*>& stack)
        {
            if (stack.empty()) {
                return;
            }

            std::string node = stack.top()->mName.C_Str();
            std::string space(stack.size() - 1, '-');

            egLOG("test", space + node);
        }

        void process_node(aiNode* node, const aiScene* scene)
        {
            transform_stack.push(convert_to_glm(node));
            nodes.push(node);
            deep = std::max(deep, transform_stack.size());
            
            print(nodes);

            // process each mesh located at the current node
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

                meshes.push_back(copy_mesh(mesh, scene));
            }

            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                process_node(node->mChildren[i], scene);
            }
            nodes.pop();
            transform_stack.pop();
        }

        Mesh copy_mesh(aiMesh* mesh, const aiScene* scene) {
            // data to fill
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            // walk through each of the mesh's vertices
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;

                glm::mat4 matrix = glm::translate(vector);
                
                for (auto trans = transform_stack; !trans.empty(); trans.pop())
                {
                    matrix = trans.top() * matrix;
                }

                glm::vec3 s;
                glm::quat o;
                glm::vec3 sk;
                glm::vec4 p;
                glm::decompose(matrix, s, o, vertex.position, sk, p);
                
                // normals
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.normal = vector;
                }
                // texture coordinates
                if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.uv = vec;
                    //// tangent
                    //vector.x = mesh->mTangents[i].x;
                    //vector.y = mesh->mTangents[i].y;
                    //vector.z = mesh->mTangents[i].z;
                    //vertex.tangent = vector;
                    //// bitangent
                    //vector.x = mesh->mBitangents[i].x;
                    //vector.y = mesh->mBitangents[i].y;
                    //vector.z = mesh->mBitangents[i].z;
                    //vertex.bitangent = vector;
                }
                else
                    vertex.uv = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);
            }
            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            // process materials
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
            // Same applies to other texture as the following list summarizes:
            // diffuse: texture_diffuseN
            // specular: texture_specularN
            // normal: texture_normalN

            Material mesh_material;
            // 1. diffuse maps
            mesh_material.diffuse = find_material_texture(material, aiTextureType_DIFFUSE);
            increase_txt_counter(mesh_material.diffuse);
            // 2. specular maps
            mesh_material.specular = find_material_texture(material, aiTextureType_SPECULAR);
            increase_txt_counter(mesh_material.specular);
            // 3. normal maps
            mesh_material.normal = find_material_texture(material, aiTextureType_HEIGHT);
            increase_txt_counter(mesh_material.normal);
            // 4. ambient maps
            mesh_material.ambient = find_material_texture(material, aiTextureType_AMBIENT);
            increase_txt_counter(mesh_material.ambient);
            meshes_counter[mesh]++;
            // return a mesh object created from the extracted mesh data
            return res::Mesh{ vertices, indices, mesh_material };
        }

        Tag find_material_texture(aiMaterial* mat, aiTextureType type) {
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                std::string_view texture_name = str.C_Str();
                return tag + Tag::make(texture_name); 
            }
            return {};
        }

        glm::mat4 convert_to_glm(aiNode* node) const {
            glm::mat4 local(1);
            aiMatrix4x4 transform = node->mTransformation;
            transform.Transpose();
            local[0] = glm::vec4(transform.a1, transform.a2, transform.a3, transform.a4);
            local[1] = glm::vec4(transform.b1, transform.b2, transform.b3, transform.b4);
            local[2] = glm::vec4(transform.c1, transform.c2, transform.c3, transform.c4);
            local[3] = glm::vec4(transform.d1, transform.d2, transform.d3, transform.d4);
            return local;
        }

        void increase_txt_counter(const res::Tag& txt)
        {
            if (txt.is_valid()) {
                textures_counter[txt]++;
            }
        }

        void batch_meshes()
        {
            std::unordered_map<std::size_t, std::vector<std::size_t>> batches;
            bool is_need_batch = false;
            for (std::size_t idx = 0; idx < meshes.size(); ++idx)
            {
                auto& mesh = meshes[idx];
                std::size_t batch_idx = mesh.material.diffuse.get_hash()
                                        | mesh.material.ambient.get_hash()
                                        | mesh.material.normal.get_hash()
                                        | mesh.material.specular.get_hash();
                batches[batch_idx].push_back(idx);
            }

            std::vector<res::Mesh> batch_result;
            for (auto& [_, bmeshes] : batches)
            {
                res::Mesh new_mesh = meshes[bmeshes[0]];
                egLOG("load/batch", "batch {}", new_mesh.material.diffuse.get_full());

                for (std::size_t idx = 1; idx < bmeshes.size(); ++idx)
                {
                    auto& mesh = meshes[bmeshes[idx]];

                    std::size_t addition = new_mesh.vertices.size();

                    new_mesh.vertices.reserve(new_mesh.vertices.size() + mesh.vertices.size());
                    std::copy(mesh.vertices.begin(), mesh.vertices.end(), std::back_inserter(new_mesh.vertices));

                    std::for_each(mesh.indices.begin(), mesh.indices.end(), [addition](unsigned int& inc) { inc += addition; });

                    new_mesh.indices.reserve(new_mesh.indices.size() + mesh.indices.size());
                    std::copy(mesh.indices.begin(), mesh.indices.end(), std::back_inserter(new_mesh.indices));
                }
                batch_result.push_back(new_mesh);
            }

            if (!batch_result.empty()) {
                egLOG("load/batching", "from {} meshes convert to {} meshes", meshes.size(), batch_result.size());

                meshes = batch_result;
            }
        }

    private:
        Tag tag;
        std::vector<Mesh> meshes;
        std::stack<glm::mat4> transform_stack;
        std::stack<aiNode*> nodes;
        std::unordered_map<res::Tag, int> textures_counter;
        std::unordered_map<aiMesh*, int> meshes_counter;
        std::size_t deep = 0;
    };
}

res::Model::Model(const Tag& tag)
	: Resource(tag)
{
    meshes_ = loader::model_loader(tag).load();
}