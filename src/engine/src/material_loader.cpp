#ifndef MATERIAL_LOADER_CPP
#define MATERIAL_LOADER_CPP

#include <iostream>
#include <fstream>
#include "material_loader.hpp"

void MaterialLoader::load_materials(std::vector<std::string> materials)
{
    for (auto material_name : materials)
    {
        MaterialLoader::get_material(material_name);
    }
}

Material *MaterialLoader::get_material(const std::string &file_path)
{

    if (MaterialLoader::pure_materials.find(file_path) != MaterialLoader::pure_materials.end())
    {
        return MaterialLoader::pure_materials[file_path];
    }

    std::string normalized_file_path = file_path;
    if (normalized_file_path.find(".json") == std::string::npos)
    {
        normalized_file_path = normalized_file_path + ".json";
    }

    std::ifstream material_file(normalized_file_path, std::ifstream::in);
    if (!material_file.good())
    {
        material_file.close();
        normalized_file_path = MaterialLoader::materials_folder + normalized_file_path;
        material_file.open(normalized_file_path, std::ifstream::in);

        if (!material_file.good())
        {
            std::cerr << "[MaterialLoader] Erro ao abrir o arquivo " << file_path.c_str() << std::endl;
            exit(1);
        }
    }

    Json::Value data;
    material_file >> data;

    Material *mtl = new Material();

    MaterialLoader::set(data, "albedo_map", mtl->diffuse_textures, TEXTURE_DIFFUSE);
    MaterialLoader::set(data, "normal_map", mtl->normal_textures, TEXTURE_NORMAL);
    MaterialLoader::set(data, "metallic_map", mtl->metallic_textures, TEXTURE_METALLIC);
    MaterialLoader::set(data, "roughness_map", mtl->roughness_textures, TEXTURE_ROUGHNESS);
    MaterialLoader::set(data, "ambient_occlusion_map", mtl->ambient_occlusion_textures, TEXTURE_AMBIENT_OCLUSION);
    MaterialLoader::set(data, "reflection_map", mtl->reflection_textures, TEXTURE_REFLECTION);

    if (data.isMember("material_color"))
    {
        Json::Value &color = data["material_color"];
        float r = color["r"].asFloat();
        float g = color["g"].asFloat();
        float b = color["b"].asFloat();
        float a = 1.0;

        if (color.isMember("a"))
        {
            a = color["a"].asFloat();
        }

        mtl->color = glm::vec4(r, g, b, a);
    }

    MaterialLoader::set(data, "metallic", mtl->metallic);
    MaterialLoader::set(data, "roughness", mtl->roughness);
    MaterialLoader::set(data, "reflection", mtl->reflection);

    /* Registra o material */
    MaterialLoader::pure_materials[file_path] = mtl;
    Material::materials.push_back(mtl);

    return mtl;
}

void MaterialLoader::set(Json::Value &data, const char *key, float &component)
{
    if (data.isMember(key))
    {
        component = data[key].asFloat();
    }
}

void MaterialLoader::set(Json::Value &data, const char *key, std::vector<Texture *> &texture_list, TextureType texture_type)
{
    if (data.isMember(key))
    {
        Texture *tex = new Texture(data[key].asString(), texture_type);
        texture_list.push_back(tex);
    }
}

const std::string MaterialLoader::materials_folder = std::string(CMAKE_ROOT_DIR MATERIAL_LOADER_FOLDER);
std::unordered_map<std::string, Material *> MaterialLoader::pure_materials;

#endif