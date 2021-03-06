#ifndef MATERIAL_CPP
#define MATERIAL_CPP

#include <iostream>
#include "texture.hpp"
#include <vector>
#include <glm/glm.hpp>
#include "material.hpp"

enum PresetMaterial
{
    PRESET_MATERIAL_WATER,
    PRESET_MATERIAL_PLASTIC,
    PRESET_MATERIAL_IRON
};

// fresnel
// Water	(0.02, 0.02, 0.02)	 (0.15, 0.15, 0.15)
// Plastic / Glass (Low)	(0.03, 0.03, 0.03)	(0.21, 0.21, 0.21)
// Plastic High	(0.05, 0.05, 0.05)	(0.24, 0.24, 0.24)
// Glass (high) / Ruby	(0.08, 0.08, 0.08)	(0.31, 0.31, 0.31)
// Diamond	(0.17, 0.17, 0.17)	(0.45, 0.45, 0.45)
// Iron	(0.56, 0.57, 0.58)	(0.77, 0.78, 0.78)
// Copper	(0.95, 0.64, 0.54)	(0.98, 0.82, 0.76)
// Gold	(1.00, 0.71, 0.29)	(1.00, 0.86, 0.57)
// Aluminium	(0.91, 0.92, 0.92)	(0.96, 0.96, 0.97)
// Silver	(0.95, 0.93, 0.88)	(0.98, 0.97, 0.95)

/*
    Texture flags
    0 - albedo
    1 - roughness
    2 - metallic
    3 - normal
    4 - ao
*/
void Material::get_texture(aiMaterial *amtl, aiTextureType texture_type, std::vector<Texture *> &texture)
{
    aiString texture_path; //contains filename of texture
    int texture_count = amtl->GetTextureCount(texture_type);

    for (int i = 0; i < texture_count; i++)
    {
        std::cerr << "TEXTURE:" << std::endl;
        std::cerr << "\t" << texture_type << std::endl;

        if (AI_SUCCESS == amtl->GetTexture(texture_type, i, &texture_path))
        {
            Texture *aux_tex = new Texture(base_path + texture_path.data, Texture::to_texture_type(texture_type));
            std::cerr << "\t" << base_path + texture_path.data << std::endl;
            std::cerr << "\t tid: " << aux_tex->id << std::endl;
            texture.push_back(aux_tex);
        }
    }
}

void Material::load_from_aiMaterial(aiMaterial *amtl, std::string base_path)
{
    float c[4];

    aiColor4D diffuse_color;
    aiColor4D specular_color;
    aiColor4D ambient_color;
    aiColor4D emission_color;
    ai_real shininess, strength;

    int ret1, ret2;
    int two_sided;
    int wireframe;
    unsigned int max; // changed: to unsigned

    int texture_index = 0;
    aiString texture_path; //contains filename of texture
    this->base_path = base_path;

    std::cerr << "material" << std::endl;

    this->get_texture(amtl, aiTextureType_DIFFUSE, this->diffuse_textures);
    this->get_texture(amtl, aiTextureType_REFLECTION, this->reflection_textures);
    this->get_texture(amtl, aiTextureType_NORMALS, this->normal_textures);
    // TODO: shininess, shininess_strength  -> metallic, roughness
    this->get_texture(amtl, aiTextureType_OPACITY, this->metallic_textures);
    this->get_texture(amtl, aiTextureType_SPECULAR, this->roughness_textures);

    // TODO: displacement map
    this->get_texture(amtl, aiTextureType_HEIGHT, this->height_textures);

    if (AI_SUCCESS == aiGetMaterialColor(amtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse_color))
    {
        std::cerr << "diffuse_color:" << std::endl;
        std::cerr << "\t" << diffuse_color[0] << std::endl;
        std::cerr << "\t" << diffuse_color[1] << std::endl;
        std::cerr << "\t" << diffuse_color[2] << std::endl;
        std::cerr << "\t" << diffuse_color[3] << std::endl;
        this->color = glm::vec4(diffuse_color[0], diffuse_color[1], diffuse_color[2], diffuse_color[3]);
    }

    max = 1;
    if (AI_SUCCESS == aiGetMaterialFloatArray(amtl, AI_MATKEY_SHININESS, &shininess, &max))
    {
        // TODO: shininess, shininess_strength  -> metallic, roughness
        // this->shininess = shininess;
    }

    max = 1;
    if (AI_SUCCESS == aiGetMaterialFloatArray(amtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max))
    {
        // TODO: shininess, shininess_strength  -> metallic, roughness
        // this->shininess_strength = strength;
    }
}

Material *Material::create_from_aiMaterial(aiMaterial *amtl, std::string base_path)
{
    Material *mtl = new Material();
    mtl->load_from_aiMaterial(amtl, base_path);

    Material::materials.push_back(mtl); // registra o material

    return mtl;
}

std::vector<Material *> Material::materials;
#endif
