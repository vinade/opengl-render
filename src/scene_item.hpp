#include "mesh.cpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>


#define SCENE_ITEM_TEX_TYPE_COUNTER 13


class SceneItem {

private:

	bool min_max_undefined = true;
	glm::vec3 max = {0.0, 0.0, 0.0};
	glm::vec3 min = {0.0, 0.0, 0.0};
	glm::vec3 center;
	glm::vec3 size;

	glm::mat4 model_matrix;
	glm::vec3 m_position = {0.0, 0.0, 0.0};
	glm::vec3 m_rotation = {0.0, 0.0, 0.0};
	glm::vec3 m_scale = {1.0, 1.0, 1.0};

	std::string model_path;
	std::string base_path;

	void collect_vertex_data(const struct aiScene *sc, const struct aiNode* nd);
	void load_scene_from_file(const std::string& file_path);
	void load_material_textures(aiMaterial* material, aiTextureType tex_type);
	void load_scene_textures();
	inline void get_min_max(aiVector3D& value, int i);
	inline void calculate_coords();

public:

	std::vector<Mesh> meshes;
	const aiScene* scene = NULL;

	static aiTextureType texture_types[];
	static Assimp::Importer assimp_importer;

	SceneItem(){};

	void load_data_from_file(const std::string& file_path);
	void draw();

	glm::mat4 get_model_matrix();
	void set_position(const glm::vec3 pos);
	void set_rotation(const glm::vec3 rotation);
	void set_scale(const glm::vec3 scale);
	void set_scale(float scale);

	void debug_coords();
};
