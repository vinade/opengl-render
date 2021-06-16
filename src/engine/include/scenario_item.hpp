#ifndef SCENARIO_ITEM_HPP
#define SCENARIO_ITEM_HPP

#include "basic_item.hpp"
#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#define SCENARIO_ITEM_MODELS_FOLDER "./res/models/"
#define SCENARIO_ITEM_TEX_TYPE_COUNTER 13

class HeightMap;

class ScenarioItem : public BasicItem
{

private:
	bool min_max_undefined = true;
	glm::vec3 max = {0.0, 0.0, 0.0};
	glm::vec3 min = {0.0, 0.0, 0.0};
	glm::vec3 center;
	glm::vec3 size;

	std::string model_path;
	std::string base_path;

	void collect_vertex_data(const struct aiScene *sc, const struct aiNode *nd);
	void load_scene_from_file(const std::string &file_path);
	void load_material_textures(aiMaterial *material, aiTextureType tex_type, bool preload);
	void load_scene_textures(bool preload);
	inline void get_min_max(aiVector3D &value, int i);
	inline void calculate_coords();

public:
	std::vector<Mesh> meshes;
	const aiScene *scene = NULL;

	static aiTextureType texture_types[];
	static Assimp::Importer assimp_importer;
	static const std::string models_folder;

	ScenarioItem(){};

	void load_data_from_file(const std::string &file_path);
	void load_data_from_file(const std::string &file_path, bool preload);
	void draw(Shader *shader);
	void draw(Shader *shader, const glm::vec3 &pos, const glm::vec3 &rotation, const glm::vec3 &scale);

	void set_material(const std::string &material_name);
	void set_material(Material *mtl);

	void set_on_height_map(HeightMap *hp);

	void debug_coords();
};

#endif
