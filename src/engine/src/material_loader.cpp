#ifndef MATERIAL_LOADER_CPP
#define MATERIAL_LOADER_CPP

#include <rapidjson/document.h>
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

    std::ifstream material_file(normalized_file_path);
    if (!material_file.good())
    {
        material_file.close();
        normalized_file_path = MaterialLoader::materials_folder + normalized_file_path;
        material_file.open(normalized_file_path);

        if (!material_file.good())
        {
            std::cerr << "[MaterialLoader] Erro ao abrir o arquivo " << file_path.c_str() << std::endl;
            exit(1);
        }
    }

    std::string file_content((std::istreambuf_iterator<char>(material_file)), std::istreambuf_iterator<char>());

    rapidjson::Document data;
    data.Parse(file_content.c_str());

    Material *mtl = new Material();

    if (data.HasMember("albedo"))
    {
        std::string texture = data["albedo"].GetString();
        mtl->diffuse_textures.push_back(new Texture(texture));
    }

    if (data.HasMember("normal"))
    {
        std::string texture = data["normal"].GetString();
        mtl->normal_textures.push_back(new Texture(texture, aiTextureType_NORMALS));
    }

    if (data.HasMember("material_color"))
    {
        rapidjson::Value &color = data["material_color"];
        float r = color["r"].GetFloat();
        float g = color["g"].GetFloat();
        float b = color["b"].GetFloat();
        float a = color["a"].GetFloat();

        mtl->material_color = glm::vec4(r, g, b, a);
    }

    if (data.HasMember("shininess"))
    {
        float shininess = data["shininess"].GetFloat();
        mtl->shininess = shininess;
    }

    /* Registra o material */
    MaterialLoader::pure_materials[file_path] = mtl;
    Material::materials.push_back(mtl);

    return mtl;
}

const std::string MaterialLoader::materials_folder = std::string(CMAKE_ROOT_DIR MATERIAL_LOADER_FOLDER);
std::unordered_map<std::string, Material *> MaterialLoader::pure_materials;

#endif