#include "texture.cpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>


#define OBJ_LOADER_TEX_TYPE_COUNTER 13


class ObjLoader {

private:

	std::string model_path;
	std::string base_path;

public:

	const aiScene* scene = NULL;

	static aiTextureType texture_types[];
	static Assimp::Importer assimp_importer;

	ObjLoader(){};

	void load_scene_from_file(const std::string& file_path);
	void load_material_textures(aiMaterial* material, aiTextureType tex_type);
	void load_scene_textures();

};
