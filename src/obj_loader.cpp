#include "obj_loader.hpp"


void ObjLoader::load_scene_from_file(const std::string& file_path){
	std::ifstream file_stream(file_path.c_str());

	if (file_stream.fail()){
		std::cerr << "[ObjLoader] erro." << std::endl;
		std::cerr << "\t" << ObjLoader::assimp_importer.GetErrorString() << std::endl;
		exit(1);
	}
	file_stream.close();

	// aiScene* scene = nullptr;
	this->scene = ObjLoader::assimp_importer.ReadFile(file_path, aiProcessPreset_TargetRealtime_Quality);
	if (!this->scene){
		std::cerr << "[ObjLoader] erro." << std::endl;
		std::cerr << "\t" << ObjLoader::assimp_importer.GetErrorString() << std::endl;
		exit(1);
	}

	this->model_path = std::string(file_path);

	size_t slash_pos = file_path.find_last_of("\\/");
	this->base_path = std::string("");
	if (std::string::npos != slash_pos){
		this->base_path = file_path.substr(0, slash_pos + 1);
	}

}


void ObjLoader::load_material_textures(aiMaterial* material, aiTextureType tex_type){
	int counter = material->GetTextureCount(tex_type);
	for (int tex_index = 0; tex_index < counter; tex_index++){
		aiString tex_path;
		material->GetTexture(tex_type, tex_index, &tex_path);
		Texture* tex = new Texture(this->base_path + tex_path.data, tex_type);
	}
}


void ObjLoader::load_scene_textures(){

	if (!this->scene){
		std::cerr << "[ObjLoader] Tentativa de carregamento de texturas sem uma cena válida." << std::endl;
		exit(1);
	}

    if (this->scene->HasTextures()){
		std::cerr << "[ObjLoader] a cena já tem texturas. ------- DEBUG." << std::endl;
    	// a cena já contém as texturas
    	return;
    }

	for (int i = 0; i < this->scene->mNumMaterials; i++)
	{
		for (int tex_type_index = 0; tex_type_index < OBJ_LOADER_TEX_TYPE_COUNTER; tex_type_index++){
			ObjLoader::load_material_textures(this->scene->mMaterials[i], ObjLoader::texture_types[tex_type_index]);
		}
	}

}


aiTextureType ObjLoader::texture_types[OBJ_LOADER_TEX_TYPE_COUNTER] = {
	aiTextureType_NONE,
	aiTextureType_DIFFUSE,
	aiTextureType_UNKNOWN,
	aiTextureType_DISPLACEMENT,
	aiTextureType_SPECULAR,
	aiTextureType_AMBIENT,
	aiTextureType_EMISSIVE,
	aiTextureType_NORMALS,
	aiTextureType_OPACITY,
	aiTextureType_SHININESS,
	aiTextureType_HEIGHT,
	aiTextureType_LIGHTMAP,
	aiTextureType_REFLECTION
};


Assimp::Importer ObjLoader::assimp_importer;

