#ifndef SCENARIO_ITEM_HPP
#define SCENARIO_ITEM_HPP

#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#ifndef CMAKE_ROOT_DIR
#define CMAKE_ROOT_DIR "./"
#endif

#define SCENARIO_ITEM_MODELS_FOLDER "./res/models/"
#define SCENARIO_ITEM_TEX_TYPE_COUNTER 13

class ScenarioItem
{

private:
	bool min_max_undefined = true;
	glm::vec3 max = {0.0, 0.0, 0.0};
	glm::vec3 min = {0.0, 0.0, 0.0};
	glm::vec3 center;
	glm::vec3 size;

	glm::vec3 m_position = {0.0, 0.0, 0.0};
	glm::vec3 m_rotation = {0.0, 0.0, 0.0};
	glm::vec3 m_scale = {1.0, 1.0, 1.0};

	std::string model_path;
	std::string base_path;

	void collect_vertex_data(const struct aiScene *sc, const struct aiNode *nd);
	void load_scene_from_file(const std::string &file_path);
	void load_material_textures(aiMaterial *material, aiTextureType tex_type);
	void load_scene_textures();
	inline void get_min_max(aiVector3D &value, int i);
	inline void calculate_coords();

public:
	glm::mat4 model_matrix;
	std::vector<Mesh> meshes;
	const aiScene *scene = NULL;

	static aiTextureType texture_types[];
	static Assimp::Importer assimp_importer;
	static const std::string models_folder;

	ScenarioItem(){};

	void load_data_from_file(const std::string &file_path);
	void draw(Shader *shader);

	void update_model_matrix();
	inline glm::vec3 get_position() { return glm::vec3(this->m_position[0], this->m_position[1], this->m_position[2]); };

	void set_position(const glm::vec3 pos);
	void set_rotation(const glm::vec3 rotation);
	void set_scale(const glm::vec3 scale);
	void set_scale(float scale);

	void inc_position(const glm::vec3 pos);
	void inc_rotation(const glm::vec3 rotation);
	void inc_scale(const glm::vec3 scale);
	void inc_scale(float scale);

	void debug_coords();
};

#endif
