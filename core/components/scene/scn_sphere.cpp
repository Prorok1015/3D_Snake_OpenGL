#include "scn_sphere.h"
#include <res_resource_model.h>
#include <rnd_render_system.h>
#include "scn_model.h"

std::vector<res::Vertex> get_sphere_data()
{
    std::vector<res::Vertex> result;

    float radius = 1.0f;
    float sectorCount = 72.f;
    float stackCount = 24.f;
    
    constexpr float PI = (float)std::numbers::pi;
    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;

    for (int i = 0; i <= stackCount; ++i)
    {
        const float stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * std::cos(stackAngle);             // r * cos(u)
        z = radius * std::sin(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            const float sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            res::Vertex vert;

            // vertex position (x, y, z)
            x = xy * std::cos(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * std::sin(sectorAngle);             // r * cos(u) * sin(v)
            vert.position_ = glm::vec3(x, y, z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vert.normal_ = glm::vec3(nx, ny, nz);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vert.tex_uv_ = glm::vec2(s, t);
            result.push_back(vert);
        }
    }

    return result;
}


void init_sphere()
{

    auto vex = get_sphere_data();

    float radius = 2.0f;
    float sectorCount = 72.f;
    float stackCount = 24.f;
    // generate CCW index list of sphere triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1
    std::vector<unsigned int> inc;
    std::vector<int> lineIndices;
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (int)(sectorCount + 1);     // beginning of current stack
        k2 = k1 + (int)sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < (int)sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                inc.push_back(k1);
                inc.push_back(k2);
                inc.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                inc.push_back(k1 + 1);
                inc.push_back(k2);
                inc.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    for (auto& v : vex) {
        v.tex_uv_ = glm::vec2(v.tex_uv_.x / 16, -v.tex_uv_.y / 16);
    }

    auto txt = rnd::get_system().get_txr_manager().require_texture(res::Tag::make("block.png"));
    txt->tmp_type = "texture_diffuse";

}

void draw_sphere(const render::Shader& ourShader)
{
}

void term_sphere()
{
}
