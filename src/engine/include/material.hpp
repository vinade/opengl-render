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
    glm::vec4 color = {0.7, 0.7, 0.7, 1.0};
    float metallic = 0.0;
    float roughness = 0.5;
    float reflection = 0.0;

    std::vector<Texture *> diffuse_textures;
    std::vector<Texture *> normal_textures;
    std::vector<Texture *> metallic_textures;
    std::vector<Texture *> roughness_textures;
    std::vector<Texture *> ambient_occlusion_textures;
    std::vector<Texture *> reflection_textures;

    std::vector<Texture *> height_textures;
    std::vector<Texture *> specular_textures;
    std::vector<Texture *> opacity_textures;

    void load_from_aiMaterial(aiMaterial *amtl, std::string base_path);
    void get_texture(aiMaterial *amtl, aiTextureType aitexture_type, std::vector<Texture *> &texture);

    static Material *create_from_aiMaterial(aiMaterial *amtl, std::string base_path);
    static std::vector<Material *> materials;
};

#endif
