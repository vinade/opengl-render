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
            std::cerr << "\t" << base_path + texture_path.data << std::endl;
            texture.push_back(new Texture(base_path + texture_path.data, texture_type));
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
    this->get_texture(amtl, aiTextureType_SPECULAR, this->specular_textures);
    this->get_texture(amtl, aiTextureType_REFLECTION, this->reflection_textures);
    this->get_texture(amtl, aiTextureType_NORMALS, this->normal_textures);
    this->get_texture(amtl, aiTextureType_HEIGHT, this->height_textures);
    this->get_texture(amtl, aiTextureType_OPACITY, this->opacity_textures);

    if (AI_SUCCESS == aiGetMaterialColor(amtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse_color))
    {
        std::cerr << "diffuse_color:" << std::endl;
        std::cerr << "\t" << diffuse_color[0] << std::endl;
        std::cerr << "\t" << diffuse_color[1] << std::endl;
        std::cerr << "\t" << diffuse_color[2] << std::endl;
        std::cerr << "\t" << diffuse_color[3] << std::endl;
        this->diffuse_color = glm::vec4(diffuse_color[0], diffuse_color[1], diffuse_color[2], diffuse_color[3]);
    }

    if (AI_SUCCESS == aiGetMaterialColor(amtl, AI_MATKEY_COLOR_SPECULAR, &specular_color))
    {
        std::cerr << "specular_color:" << std::endl;
        std::cerr << "\t" << specular_color[0] << std::endl;
        std::cerr << "\t" << specular_color[1] << std::endl;
        std::cerr << "\t" << specular_color[2] << std::endl;
        std::cerr << "\t" << specular_color[3] << std::endl;
        this->specular_color = glm::vec4(specular_color[0], specular_color[1], specular_color[2], specular_color[3]);
    }

    if (AI_SUCCESS == aiGetMaterialColor(amtl, AI_MATKEY_COLOR_AMBIENT, &ambient_color))
    {
        std::cerr << "ambient_color:" << std::endl;
        std::cerr << "\t" << ambient_color[0] << std::endl;
        std::cerr << "\t" << ambient_color[1] << std::endl;
        std::cerr << "\t" << ambient_color[2] << std::endl;
        std::cerr << "\t" << ambient_color[3] << std::endl;
        this->ambient_color = glm::vec4(ambient_color[0], ambient_color[1], ambient_color[2], ambient_color[3]);
    }

    if (AI_SUCCESS == aiGetMaterialColor(amtl, AI_MATKEY_COLOR_EMISSIVE, &emission_color))
    {
        std::cerr << "emission_color:" << std::endl;
        std::cerr << "\t" << emission_color[0] << std::endl;
        std::cerr << "\t" << emission_color[1] << std::endl;
        std::cerr << "\t" << emission_color[2] << std::endl;
        std::cerr << "\t" << emission_color[3] << std::endl;
        this->emission_color = glm::vec4(emission_color[0], emission_color[1], emission_color[2], emission_color[3]);
    }

    max = 1;
    if (AI_SUCCESS == aiGetMaterialFloatArray(amtl, AI_MATKEY_SHININESS, &shininess, &max))
    {
        this->shininess = shininess;
    }

    max = 1;
    if (AI_SUCCESS == aiGetMaterialFloatArray(amtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max))
    {
        this->shininess_strength = strength;
    }
    std::cerr << "shininess:" << this->shininess << std::endl;
    std::cerr << "shininess_strength:" << this->shininess_strength << std::endl;
}

#endif
