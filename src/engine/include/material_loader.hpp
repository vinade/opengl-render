#ifndef MATERIAL_LOADER_HPP
#define MATERIAL_LOADER_HPP

#include "cmake_params.hpp"
#include "material.hpp"

#define MATERIAL_LOADER_FOLDER "./res/materials/"

class MaterialLoader
{
public:
    static const std::string materials_folder;
    static std::unordered_map<std::string, Material *> pure_materials;
    static Material *get_material(const std::string &file_path);
    static void load_materials(std::vector<std::string> materials);
};

#endif