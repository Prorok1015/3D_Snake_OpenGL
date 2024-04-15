#include "mesh.h"
#include "../render/rnd_render_system.h"

scene::Mesh::Mesh(std::vector<res::Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<rnd::Texture>> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    setupMesh();
}

scene::Mesh::~Mesh()
{
    rnd::get_system().del_buf(EBO);
    rnd::get_system().del_buf(VBO);
    rnd::get_system().del_vertex_buf(VAO);
}

void scene::Mesh::draw(const rnd::Shader& shader)
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i]->tmp_type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        shader.uniform(name + number, (int)i);

        rnd::get_system().activate_texture_unit(i);
        // and finally bind the texture
        textures[i]->bind();
    }

    // draw mesh
    rnd::get_system().draw_elements(VAO, static_cast<unsigned int>(indices.size()));

    // always good practice to set everything back to defaults once configured.
    rnd::get_system().reset_texture();
}

void scene::Mesh::setupMesh()
{
    // create buffers/arrays
    VAO = rnd::get_system().gen_vertex_buf();
    VBO = rnd::get_system().gen_buf();
    EBO = rnd::get_system().gen_buf();

    rnd::get_system().bind_vertex_array(VAO);
    // load data into vertex buffers
    rnd::get_system().bind_buffer(VBO, GL_ARRAY_BUFFER);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    rnd::get_system().buffer_data(GL_ARRAY_BUFFER, vertices.size() * sizeof(res::Vertex), &vertices[0]);

    rnd::get_system().bind_buffer(EBO, GL_ELEMENT_ARRAY_BUFFER);
    rnd::get_system().buffer_data(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0]);

    // set the vertex attribute pointers
    // vertex Positions
    rnd::get_system().vertex_attribute_pointer(0, 3, GL_FLOAT, sizeof(res::Vertex), (void*)0);
    // vertex normals
    rnd::get_system().vertex_attribute_pointer(1, 3, GL_FLOAT, sizeof(res::Vertex), (void*)offsetof(res::Vertex, normal_));
    // vertex texture coords
    rnd::get_system().vertex_attribute_pointer(2, 2, GL_FLOAT, sizeof(res::Vertex), (void*)offsetof(res::Vertex, texture_position_));
    // vertex tangent
    rnd::get_system().vertex_attribute_pointer(3, 3, GL_FLOAT,  sizeof(res::Vertex), (void*)offsetof(res::Vertex, tangent_));
    // vertex bitangent
    rnd::get_system().vertex_attribute_pointer(4, 3, GL_FLOAT, sizeof(res::Vertex), (void*)offsetof(res::Vertex, bitangent_));
    // ids
    rnd::get_system().vertex_attribute_pointeri(5, 4, GL_INT, sizeof(res::Vertex), (void*)offsetof(res::Vertex, bones_));

    // weights
    rnd::get_system().vertex_attribute_pointer(6, 4, GL_FLOAT, sizeof(res::Vertex), (void*)offsetof(res::Vertex, bones_weight_));
    rnd::get_system().bind_vertex_array(0);
}