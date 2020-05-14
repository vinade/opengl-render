#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "texture.hpp"
#include <vector>
#include <glm/glm.hpp>

class Material
{

public:
    std::string base_path;

    // quando não tiver mappping, um parâmtero será aplicado para todo o objeto
    glm::vec4 diffuse_color = {0.7, 0.7, 0.7, 1.0}; // color, se não tiver albedo
    glm::vec4 specular_color = {1.0, 1.0, 1.0, 1.0};
    glm::vec4 ambient_color = {1.0, 1.0, 1.0, 1.0};
    glm::vec4 emission_color = {1.0, 1.0, 1.0, 1.0};

    std::vector<Texture *> diffuse_textures;
    std::vector<Texture *> normal_textures;
    std::vector<Texture *> height_textures;
    std::vector<Texture *> specular_textures;
    std::vector<Texture *> reflection_textures;
    std::vector<Texture *> opacity_textures;

    // roughness diminui fresnel?
    // glm::vec3 fresnelF0_factor = {0.03, 0.03, 0.03};
    float shininess = 20.0;
    float shininess_strength = 1.0;

    // float normal_factor = 0.9;                // geometryfunction
    // glm::vec3 ao_factor = {0.03, 0.03, 0.03}; // geometryfunction
    // float reflectance = 0.0;                  // ....

    // // fr = k*(lambert)*flambert + k*(1-lambert)*fcooktorrance
    // float lambert = 0.0; // cook-torrance  = (1.0 - lambert)

    void load_from_aiMaterial(aiMaterial *amtl, std::string base_path);
    void get_texture(aiMaterial *amtl, aiTextureType texture_type, std::vector<Texture *> &texture);
};

#endif
